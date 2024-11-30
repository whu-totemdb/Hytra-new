package edu.whu.hytra.core;

import lombok.extern.slf4j.Slf4j;

import java.io.*;
import java.net.Socket;

@Slf4j
public class SocketStorageManager implements StorageManager {

    private static final String SUCCESS = "Insert OK!";
    private Socket s;

    public SocketStorageManager(Socket socket) {
        this.s = socket;
    }
    

    @Override
    public void put(String key, String value) throws Exception {
        write("put:" + key + "," + value);
        String code = read();
        if (!SUCCESS.equals(code)) {
            throw new IOException("error sending command," + code);
        }
    }

    @Override
    public String get(String key) throws Exception {
        write("get:" + key);
        return read();
    }

    public String status() throws Exception {
        write("status");
        return read();
    }

    @Override
    public boolean config(String key, String path) throws Exception {
        write("config:" + key + "," + path);
        String result = read();
        return SUCCESS.equals(result);
    }

    private void write(String msg) throws Exception {
        OutputStream os = s.getOutputStream();
        BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(os));
        bw.write(msg);
        log.debug("[socket]" + msg);
        bw.flush();
    }

    private String read() throws Exception {
        InputStream is = s.getInputStream();
        BufferedReader br = new BufferedReader(new InputStreamReader(is));
        String mess = br.readLine();
        log.debug("[socket]receive " + mess);
        return mess;
    }

    @Override
    public void close() throws Exception {
        write("exit");
        s.close();
    }
}
