package edu.whu.hytra.core;

import java.net.Socket;

public interface StorageManager {

    /**
     * 插入数据
     */
    public void put(String key, String value) throws Exception;

    /**
     * 获取指定 key 对应的值
     */
    public String get(String key) throws Exception;

    public String status() throws Exception;

    public boolean config(String key, String path) throws Exception;

    public void close() throws Exception;
}
