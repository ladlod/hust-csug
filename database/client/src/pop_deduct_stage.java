package sample;

import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.stage.Stage;

import java.io.*;
import java.net.Socket;

/**
 * Created by ladlod on 19-5-26.
 */
public class pop_deduct_stage {
    public void start(Stage primaryStage, BufferedReader bin, PrintWriter pw) throws Exception{
        agreement A = new agreement();
        Parent deduct_p = FXMLLoader.load(getClass().getResource("deduct.fxml"));
        Scene deduct_s = new Scene(deduct_p);
        Button btn0 = (Button)deduct_p.lookup("#btn_0");
        Button btn1 = (Button)deduct_p.lookup("#btn_1");
        TextField id = (TextField)deduct_p.lookup("#id");
        TextField money = (TextField)deduct_p.lookup("#money");
        Label label = (Label)deduct_p.lookup("#label");

        btn0.setOnAction(e->{
            String uid = id.getText();
            String umoney = money.getText();
            String [] spString = new String[10];
            spString[0] = "deduct";
            spString[1] = uid;
            spString[2] = umoney;
            A.deal_s(spString);
            //System.out.println(A.result);
            pw.println(A.result);
            pw.flush();
            try {
                if (bin.readLine().equals("true")) label.setText("扣费成功！");
                else label.setText("用户不存在！");
            }catch(Exception e0){
                e0.printStackTrace();
            }
        });

        btn1.setOnAction(e-> primaryStage.close());

        primaryStage.setTitle("扣费");
        primaryStage.setScene(deduct_s);
        primaryStage.show();
    }
}
