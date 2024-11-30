package edu.whu.hytra.core;

import org.junit.Before;
import org.junit.Test;

import java.io.IOException;
import java.net.Socket;
import java.net.UnknownHostException;

public class SocketStorageManagerTest {

    private Socket socket;

    @Before
    public void initSocket() {
        try {
            this.socket = new Socket("127.0.0.1", 9200);
        } catch (UnknownHostException e) {
            System.err.println("Cannot resolve host");
        } catch (IOException e) {
            System.err.println("IO Exception on socket");
        }
    }

    public void closeSocket() {
        try {
            this.socket.close();
        } catch (IOException e) {
            System.out.println("Close socket error:" + e);
        }
    }

    @Test
    public void putTest() throws Exception {
        SocketStorageManager mng = new SocketStorageManager(socket);
        mng.put("a", "1");
        mng.put("a", "2");
        mng.put("a", "3");
        mng.close();
    }

    @Test
    public void getTest() throws Exception {
        SocketStorageManager mng = new SocketStorageManager(socket);
        mng.put("2023-04-02@123456@0", "1");
        mng.put("2023-04-02@123456@1", "2");
        mng.put("2023-04-02@123456@2", "3");
        String result = mng.get("2023-04-02@123456@0");
        assert "1".equals(result);
        mng.close();
    }
}
