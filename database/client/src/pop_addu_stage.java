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
 * Created by ladlod on 19-5-26.
 */
public class pop_addu_stage {
    public void start(Stage primaryStage, Integer cid, BufferedReader bin, PrintWriter pw) throws Exception{
        agreement A = new agreement();

        Parent addu_p = FXMLLoader.load(getClass().getResource("addu.fxml"));
        Scene addu_s = new Scene(addu_p);
        Button btn0 = (Button) addu_p.lookup("#btn_0");
        Button btn1 = (Button) addu_p.lookup("#btn_1");
        TextField uid_t = (TextField) addu_p.lookup("#uid");
        TextField uname_t = (TextField) addu_p.lookup("#uname");
        TextField meal_t = (TextField) addu_p.lookup("#meal");
        TextField money_t = (TextField) addu_p.lookup("#money");
        PasswordField pwd_t = (PasswordField) addu_p.lookup("#upw");
        PasswordField pwd_t2 = (PasswordField) addu_p.lookup("#upw_2") ;
        Label label = (Label) addu_p.lookup("#label");

        btn0.setOnAction(e -> {
            String uid = uid_t.getText();
            String uname = uname_t.getText();
            String meal = meal_t.getText();
            String money = money_t.getText();
            String pwd = pwd_t.getText();
            String pwd2 = pwd_t2.getText();
            if(pwd.equals(pwd2)) {
                String[] spString = new String[10];
                spString[0] = "add_user";
                spString[1] = uid;
                spString[2] = uname;
                spString[3] = meal;
                spString[4] = money;
                spString[5] = "正常";
                spString[6] = cid.toString();
                spString[7] = pwd;
                A.deal_s(spString);
                //System.out.println(A.result);
                pw.println(A.result);
                pw.flush();
                try {
                    if (bin.readLine().equals("true")) label.setText("   开户成功！");
                    else label.setText("   id已存在！");
                } catch (Exception e0) {
                    e0.printStackTrace();
                }
            }
            else{
                label.setText("两次输入密码不同！");
            }

        });

        btn1.setOnAction(e -> primaryStage.close());

        primaryStage.setTitle("开户");
        primaryStage.setScene(addu_s);
        primaryStage.show();
    }
}
