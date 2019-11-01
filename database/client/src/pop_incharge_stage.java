package sample;
import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Label;
import javafx.scene.control.PasswordField;
import javafx.scene.control.TextField;
import javafx.stage.Stage;
import javafx.scene.control.Button;

import java.io.*;
import java.net.Socket;

/**
 * Created by ladlod on 19-5-26.
 */
public class pop_incharge_stage {
    public void start(Stage primaryStage, BufferedReader bin, PrintWriter pw) throws Exception{
        agreement A = new agreement();

        Parent incharge_p = FXMLLoader.load(getClass().getResource("incharge.fxml"));
        Scene incharge_s = new Scene(incharge_p);
        Button btn0 = (Button)incharge_p.lookup("#btn_0");
        Button btn1 = (Button)incharge_p.lookup("#btn_1");
        TextField id = (TextField)incharge_p.lookup("#id");
        TextField money = (TextField)incharge_p.lookup("#money");
        Label label = (Label)incharge_p.lookup("#label");

        btn0.setOnAction(e->{
            String uid = id.getText();
            String umoney = money.getText();
            String [] spString = new String[10];
            spString[0] = "incharge";
            spString[1] = uid;
            spString[2] = umoney;
            A.deal_s(spString);
            //System.out.println(A.result);
            pw.println(A.result);
            pw.flush();
            try {
                if (bin.readLine().equals("true")) label.setText("缴费成功！");
                else label.setText("用户不存在！");
            }catch(Exception e0){
                e0.printStackTrace();
            }
        });

        btn1.setOnAction(e-> primaryStage.close());

        primaryStage.setTitle("缴费");
        primaryStage.setScene(incharge_s);
        primaryStage.show();
    }
}
