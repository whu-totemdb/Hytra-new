package edu.whu.hyk;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.stream.JsonReader;
import edu.whu.hytra.entity.Vehicle;
import org.junit.Test;

import java.io.*;
import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class DBTest {

    private Gson gson;

    public DBTest() {
        gson = new GsonBuilder().create();
    }

    public static String gtfs_file = "gtfs.json";

    //    @Test
    public void build() {
        Connection conn = null;
        try {
            Class.forName("org.sqlite.JDBC");
            conn = DriverManager.getConnection("jdbc:sqlite:syd.db");
            String sql = "select * from sample limit 100;";
            try (PreparedStatement pstmt = conn.prepareStatement(sql)) {
                ResultSet rs = pstmt.executeQuery();
                if (rs.isClosed()) {
                    System.out.println("no result is found!");
                }
                while (rs.next()) {
                    System.out.println(rs.getInt("pid"));
                }
                rs.close();
            } catch (SQLException e) {
                throw new IllegalStateException(e.getMessage());

            }
        } catch (Exception e) {
            System.err.println(e.getClass().getName() + ": " + e.getMessage());
            System.exit(0);
        }
        System.out.println("Opened database successfully");
    }

    public List<Vehicle> readGtfsData() {
        List<Vehicle> list = new ArrayList<>();
        try {
//            String path = this.getClass().getClassLoader().getResource("/").getPath();
//            System.out.println(path);
            System.out.println(System.getProperty("user.dir"));
            JsonReader reader = new JsonReader(new InputStreamReader(new FileInputStream(System.getProperty("user.dir") + "/src/test/data/gtfs.json"), "UTF-8"));
            reader.beginArray();
            while (reader.hasNext()) {
                Vehicle v = gson.fromJson(reader, Vehicle.class);
                list.add(v);
            }
        } catch (UnsupportedEncodingException ex) {
            System.out.println("UnsupportedEncodingException");
        } catch (IOException ex) {
            System.out.println("IOException");
        }
        return list;
    }

    @Test
    public void TestReadGtfsData() {
        List<Vehicle> list = readGtfsData();
        assert list.size() != 0;
    }
}
