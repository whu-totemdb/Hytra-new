package edu.whu.hytra;

public class EngineParam {
    public EngineParam(String city, double[] spatialDomain, int resolution, String seperator, int epsilon, int dataSize) {
        this.city = city;
        this.spatialDomain = spatialDomain;
        this.resolution = resolution;
        this.seperator = seperator;
        this.epsilon = epsilon;
        this.dataSize = dataSize;
    }

    // 城市名
    private String city;
    // 空间范围，框定一个矩形范围
    private double[] spatialDomain;
    // 地图放大倍数
    private int resolution;
    // 数据分割符
    private String seperator;
    //
    private int epsilon;
    // 数据大小
    private int dataSize;

    public String getCity() {
        return city;
    }

    public void setCity(String city) {
        this.city = city;
    }

    public double[] getSpatialDomain() {
        return spatialDomain;
    }

    public void setSpatialDomain(double[] spatialDomain) {
        this.spatialDomain = spatialDomain;
    }

    public int getResolution() {
        return resolution;
    }

    public void setResolution(int resolution) {
        this.resolution = resolution;
    }

    public String getSeperator() {
        return seperator;
    }

    public void setSeperator(String seperator) {
        this.seperator = seperator;
    }

    public int getEpsilon() {
        return epsilon;
    }

    public void setEpsilon(int epsilon) {
        this.epsilon = epsilon;
    }

    public int getDataSize() {
        return dataSize;
    }

    public void setDataSize(int dataSize) {
        this.dataSize = dataSize;
    }
}
