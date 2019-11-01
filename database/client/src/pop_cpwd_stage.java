package sample;

import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.PasswordField;
import javafx.scene.control.TextField;
import javafx.stage.Stage;

import java.io.BufferedReader;
import java.io.PrintWriter;

/**
 * Created by ladlod on 19-5-27.
 */
public class pop_cpwd_stage {
    public void start(Stage primaryStage, Integer uid, BufferedReader bin, PrintWriter pw) throws Exception{
        agreement A = new agreement();

        Parent cpwd_p = FXMLLoader.load(getClass().getResource("change_pwd.fxml"));
        Scene cpwd_s = new Scene(cpwd_p);
        Button btn0 = (Button)cpwd_p.lookup("#btn_0");
        Button btn1 = (Button)cpwd_p.lookup("#btn_1");
        PasswordField pwd_t = (PasswordField) cpwd_p.lookup("#pwd");
        PasswordField pwd_t2 = (PasswordField) cpwd_p.lookup("#pwd2") ;
        Label label = (Label)cpwd_p.lookup("#label");

        btn0.setOnAction(e->{
            String pwd = pwd_t.getText();
            String pwd2 = pwd_t2.getText();
            if(pwd.equals(pwd2)) {
                String[] spString = new String[10];
                spString[0] = "change_password";
                spString[1] = uid.toString();
                spString[2] = pwd;
                A.deal_s(spString);
                //System.out.println(A.result);
                pw.println(A.result);
                pw.flush();
                try {
                    if (bin.readLine().equals("true")) label.setText("修改成功！");
                    else label.setText("修改失败");
                } catch (Exception e0) {
                    e0.printStackTrace();
                }
            }
            else{
                label.setText("两次输入密码不同！");
            }
        });


        btn1.setOnAction(e-> primaryStage.close());

        primaryStage.setTitle("修改密码");
        primaryStage.setScene(cpwd_s);
        primaryStage.show();
    }
}
