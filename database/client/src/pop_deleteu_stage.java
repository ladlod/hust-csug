package sample;

import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.PasswordField;
import javafx.scene.control.TextField;
import javafx.stage.Stage;

import java.io.*;
import java.net.Socket;

/**
 * Created by ladlod on 19-5-27.
 */
public class pop_deleteu_stage {
    public void start(Stage primaryStage, BufferedReader bin, PrintWriter pw) throws Exception{
        agreement A = new agreement();

        Parent deleteu_p = FXMLLoader.load(getClass().getResource("deleteu.fxml"));
        Scene deleteu_s = new Scene(deleteu_p);
        Button btn0 = (Button) deleteu_p.lookup("#btn_0");
        Button btn1 = (Button) deleteu_p.lookup("#btn_1");
        TextField uid_t = (TextField) deleteu_p.lookup("#id");
        Label label = (Label) deleteu_p.lookup("#label");

        btn0.setOnAction(e -> {
            String uid = uid_t.getText();
            String[] spString = new String[10];
            spString[0] = "delete_user";
            spString[1] = uid;
            A.deal_s(spString);
            pw.println(A.result);
            pw.flush();
            try {
                if (bin.readLine().equals("true")) label.setText("销户成功！");
                else label.setText("id不存在！");
            } catch (Exception e0) {
                e0.printStackTrace();
            }
        });

        btn1.setOnAction(e -> primaryStage.close());

        primaryStage.setTitle("销户");
        primaryStage.setScene(deleteu_s);
        primaryStage.show();
    }
}
