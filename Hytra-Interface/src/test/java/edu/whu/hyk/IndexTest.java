package edu.whu.hyk;


import edu.whu.hytra.EngineFactory;
import edu.whu.hytra.entity.Vehicle;
import edu.whu.hyk.model.Point;
import edu.whu.hyk.model.PostingList;
import org.junit.Before;
import org.junit.Test;

import java.time.LocalDateTime;
import java.time.ZoneOffset;
import java.time.format.DateTimeFormatter;
import java.util.List;
import java.util.Random;
import java.util.stream.Collectors;


public class IndexTest {

    private List<Vehicle> dataList;

    private AppTest app;

    @Before
    public void prepare() {
        app = new AppTest();
        DBTest test = new DBTest();
        dataList = test.readGtfsData();
        assert dataList.size() != 0;
    }

    private void updateIndex() {
        assert dataList.size() != 0;
        List<Point> parsedList = dataList.stream().filter(p -> p.getTID() != 0).map(p -> new Point(p.getId().hashCode(), p.getLat(), p.getLon(), LocalDateTime.ofEpochSecond(p.getRecordedTime(), 0, ZoneOffset.ofHours(-5)).format(DateTimeFormatter.ofPattern("yyyy-MM-dd hh:mm:ss")), p.getTripID().hashCode())
        ).collect(Collectors.toList());
        Engine.buildIndex(parsedList);
    }

    @Test
    public void TestUpdateIndex() {
        updateIndex();
        assert !PostingList.TC.isEmpty();
//        assert !PostingList.TP.isEmpty();
        assert !PostingList.TlP.isEmpty();
        assert !PostingList.GT.isEmpty();
        assert !PostingList.CT.isEmpty();
        assert !PostingList.CP.isEmpty();
        assert !Engine.TG.isEmpty();
    }

    @Test
    public void TestSearchIndex() {
        updateIndex();
        Random rand = new Random();
        int randIndex = rand.nextInt(PostingList.TlP.size());
        int tid = (Integer) PostingList.TlP.keySet().toArray()[randIndex];
        int pid = PostingList.TlP.get(tid);
        List<Point> points = Engine.trajDataBase.get(tid);
        Point latestPoint = points.get(points.size() - 1);
        assert latestPoint.getPid() == pid;
        double targetPointLat = latestPoint.getLat();
        double targetPointLon = latestPoint.getLon();
        double queryLat = targetPointLat + 0.00001;
        double queryLon = targetPointLon - 0.00001;
        System.out.println(String.format("Target Point is (%f, %f)", queryLat, queryLon));
        List<Integer> result = app.engineFactory.searchRealtime(queryLat, queryLon, 10);
        for (Integer i : result) {
            if (i == tid) {
                return;
            }
        }
        assert false;
    }

}
