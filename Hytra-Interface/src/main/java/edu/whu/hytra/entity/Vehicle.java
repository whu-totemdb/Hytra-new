package edu.whu.hytra.entity;

import java.time.LocalDateTime;

/**
 * 标准格式的 gtfs 的实时数据结构
 */
public class Vehicle {
    /*
    车辆 ID
     */
    private String id;
    /*
    服务提供商的 ID
     */
//    private String agencyID;

    /*
    预计到达时间
     */
//    private long aimedArrivalTime;
    /*
    TODO
     */
//    private float bearing;

    /*
    当前车辆行驶路线方向：正向 or 逆向
     */
//    private String direction;

    /*
    到达下一站的距离
     */
//    private float distanceFromNextStop;

    /*
    距离起点的距离
     */
//    private float distanceFromOrigin;

    private double lat;

    private double lon;

    /*
    下一站的名称
     */
//    private String nextStop;

    /*
    起点站的名称
     */
//    private String originStop;

    /*
    TODO: 意义不明
     */
//    private int presentableDistance;

    private long recordedTime;

    /*
    当前轨迹的 ID
     */
//    private String routeID;

    /*
    当前行程的 ID
     */
    private String tripID;

//    private float speed;
//    private long lastUpdate;


    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public double getLat() {
        return lat;
    }

    public void setLat(double lat) {
        this.lat = lat;
    }

    public double getLon() {
        return lon;
    }

    public void setLon(double lon) {
        this.lon = lon;
    }

    public long getRecordedTime() {
        return recordedTime;
    }

    public void setRecordedTime(long recordedTime) {
        this.recordedTime = recordedTime;
    }

    public String getTripID() {
        return tripID;
    }

    public void setTripID(String tripID) {
        this.tripID = tripID;
    }

    public Integer getPID() {
        return id.hashCode();
    }

    public Integer getTID() {
        return tripID.hashCode();
    }

}

