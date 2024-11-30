package edu.whu.hyk;

import edu.whu.hyk.encoding.Encoder;
import edu.whu.hyk.oldexp.*;
import edu.whu.hyk.merge.Generator;
import edu.whu.hyk.model.Point;
import edu.whu.hyk.model.PostingList;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.sql.*;
import java.util.*;

public class Engine {
    public static HashMap<String, Object> Params = new HashMap();

    /**
     * 用来读取文本数据的时候保存一下数据，目前在各个模型的 setup 中仍然用到这个作为传入的数据
     */
    public static HashMap<Integer, List<Point>> trajDataBase = new HashMap<>();

    //用于相似度查询
    static HashMap<Integer, List<Integer>> TG = new HashMap<>();
    static HashMap<Integer, List<Integer>> GT = new HashMap<>();


    public static void main(String[] args) throws IOException {
        //纽约参数
        Params.put("city", "nyc");
        Params.put("spatialDomain", new double[]{40.502873, -74.252339, 40.93372, -73.701241});
        Params.put("resolution", 6);
        Params.put("separator", "@");
        Params.put("epsilon", 30);
        Params.put("dataSize", (int) 1.2e7);

        //悉尼参数
//        Params.put("city","sydney");
//        Params.put("spatialDomain", new double[]{-34,150.6,-33.6,151.3});
//        Params.put("resolution",6);
//        Params.put("separator", "@");
//        Params.put("epsilon",30);
//        Params.put("dataSize",(int)16e6);

        Encoder.setup(Params);
        Generator.setup(Params);

        //real-time range query
        //1：需要先缓存traj database（并构建GT和TlP）
        buildTrajDB((String) Params.get("city"), "jun");
        //2：设置查询参数：query range
        RealtimeRange.setup(trajDataBase, Params, 3000);
        //3: 执行查询
        RealtimeRange.hytra(PostingList.GT, PostingList.TlP);


    }

    public static void buildTrajDB(String city, String tableName) {
        Connection conn = null;
        List<Point> newPoints = new ArrayList<>();
        try {
            Class.forName("org.sqlite.JDBC");
            conn = DriverManager.getConnection(String.format("jdbc:sqlite:your_path_here", city.substring(0, 3)));
            int dataSize = (int) Params.get("dataSize");
            String sql = String.format("select * from %s limit %d", tableName, (int) Params.get("dataSize"));
            if (dataSize == -1) {
                sql = String.format("select * from %s", tableName);
            }
            try (PreparedStatement pstmt = conn.prepareStatement(sql)) {
                ResultSet rs = pstmt.executeQuery();
                if (rs.isClosed()) {
                    System.out.println("no result is found!");
                }
                while (rs.next()) {
                    int pid = rs.getInt("pid");
                    double lat = rs.getDouble("lat");
                    double lon = rs.getDouble("lon");
                    String datetime = rs.getString("datetime");
                    int tid = rs.getInt("tid");
                    Point p = new Point(pid, lat, lon, datetime, tid);
                    newPoints.add(p);
                }
                rs.close();
            } catch (SQLException e) {
                throw new IllegalStateException(e.getMessage());

            }
            // 数据写入内存索引
            buildIndex(newPoints);
        } catch (Exception e) {
            System.err.println(e.getClass().getName() + ": " + e.getMessage());
            System.exit(0);
        }
        System.out.println("Opened database successfully");
    }

    /**
     * 写入索引，需要提供对应的轨迹 ID，使用特定的转换规则：tripID 可以决定唯一的轨迹
     *
     * @param points 所有的点列表，包含：位置信息、点 ID、位置 ID、时间点
     */
    public static void buildIndex(List<Point> points) {
        for (Point p : points) {
            int pid = p.getPid();
            int tid = p.getTid();
            // 1. 写入 t-p 的完整 map 作为数据源
            if (!trajDataBase.containsKey(tid)) {
                trajDataBase.put(tid, new ArrayList<Point>());
            }
            trajDataBase.get(tid).add(p);

            PostingList.TlP.put(tid, pid);
            // 2. 编码时间相关网格
            String cid = Encoder.encodeCube(p);
            if (cid != null) {
                //CP
                HashSet<Integer> pidSet = PostingList.CP.getOrDefault(cid, new HashSet<>());
                pidSet.add(pid);
                PostingList.CP.put(cid, pidSet);
                //CT
                HashSet<Integer> tidSet = PostingList.CT.getOrDefault(cid, new HashSet<>());
                tidSet.add(tid);
                PostingList.CT.put(cid, tidSet);
                //TC
                List<String> cidSet = PostingList.TC.getOrDefault(tid, new ArrayList<>());
                cidSet.add(cid);
                PostingList.TC.put(tid, cidSet);
            }
            // 3. 编码时间无关网格
            int gid = Encoder.encodeGrid(p.getLat(), p.getLon());
            if (!PostingList.GT.containsKey(gid)) {
                PostingList.GT.put(gid, new HashSet<>());
            }
            PostingList.GT.get(gid).add(tid);
            // 这里的 TG 需要用来加速 GT 的更新，这里 TG 保存了历史所有的网格 id，有冗余，需要优化
            if (!TG.containsKey(tid)) {
                TG.put(tid, new ArrayList<>());
                TG.get(tid).add(gid);
            }
            // TODO 遍历四周 Grid 去删除 tid。

            int size = TG.get(tid).size();
            // 轨迹对应的网格更新了，需要更新 GT
            if (!Objects.equals(gid, TG.get(tid).get(size - 1))) {
                TG.get(tid).add(gid);

            }

            List<Integer> gidList = TG.getOrDefault(tid, new ArrayList<>());
            gidList.add(gid);
            TG.put(tid, gidList);

            List<Integer> tidList = GT.getOrDefault(gid, new ArrayList<>());
            tidList.add(tid);
            GT.put(gid, tidList);
        }
    }

    /**
     * 写入索引，使用特定的转换规则：tripID 可以决定唯一的轨迹
     *
     * @param data CTList
     */
    public static void buildCTIndex(HashMap<String, ArrayList<String>> data) {
        for (String cubeId : data.keySet()) {
            ArrayList<String> tripIdList = data.get(cubeId);
            HashSet<Integer> tripIdIntList = new HashSet<>();

            for (String s : tripIdList) {
                tripIdIntList.add(s.hashCode());
            }

            PostingList.CT.put(cubeId, tripIdIntList);
        }
    }

    public static void writeTC(String filePath) {
        File f = new File(filePath);
        FileOutputStream out;
        try {
            out = new FileOutputStream(f, false);
            OutputStreamWriter writer = new OutputStreamWriter(out, "UTF-8");
            StringBuilder sb = new StringBuilder();
            PostingList.TC.entrySet().forEach(entry -> sb.append(entry).append("\n"));
            writer.write(sb.toString());
            writer.close();

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void clear() {
        PostingList.TlP.clear();
        trajDataBase.clear();
        PostingList.CP.clear();
        PostingList.CT.clear();
        PostingList.TC.clear();
        PostingList.GT.clear();
        GT.clear();
        TG.clear();
        PostingList.mergeCT.clear();
        PostingList.mergeTC.clear();
        PostingList.mergeCP.clear();
    }

    public static void writeTG(String filePath) {
        File f = new File(filePath);
        FileOutputStream out;
        try {
            out = new FileOutputStream(f, false);
            OutputStreamWriter writer = new OutputStreamWriter(out, "UTF-8");
            StringBuilder sb = new StringBuilder();
            TG.entrySet().forEach(entry -> sb.append(entry).append("\n"));
            writer.write(sb.toString());
            writer.close();

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void writeTCWithCompaction(String filePath) {
        File f = new File(filePath);
        FileOutputStream out;
        try {
            out = new FileOutputStream(f, false);
            OutputStreamWriter writer = new OutputStreamWriter(out, "UTF-8");
            StringBuilder sb = new StringBuilder();
            PostingList.TC.entrySet().forEach(entry -> sb.append(entry).append("\n"));
            writer.write(sb.toString());
            writer.close();

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

}
