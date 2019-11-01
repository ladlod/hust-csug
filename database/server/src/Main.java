import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.file.attribute.UserDefinedFileAttributeView;
import java.util.concurrent.Executors;
import java.util.concurrent.ExecutorService;

public class Main {
    public static void main(String[] args) {
        ExecutorService pool = null;
        pool = Executors.newFixedThreadPool(10);
        ServerSocket serversocket = null;
        try {
            serversocket = new ServerSocket(10010);
        }catch (Exception e){
            e.printStackTrace();
        }
        while (true) {
            Socket socket = null;
            try {
                socket = serversocket.accept();
            } catch (Exception e) {
                e.printStackTrace();
            }
            InnerThread innerThread = new InnerThread(socket);
            pool.execute(innerThread);
        }
    }
}
