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
public class pop_addca_stage {
    public void start(Stage primaryStage, BufferedReader bin, PrintWriter pw) throws Exception{
        agreement A = new agreement();

        Parent addca_p = FXMLLoader.load(getClass().getResource("addca.fxml"));
        Scene addca_s = new Scene(addca_p);
        Button btn0 = (Button) addca_p.lookup("#btn_0");
        Button btn1 = (Button) addca_p.lookup("#btn_1");
        TextField caid_t = (TextField) addca_p.lookup("#cid");
        TextField caname_t = (TextField) addca_p.lookup("#cname");
        PasswordField pwd_t = (PasswordField) addca_p.lookup("#cpw");
        PasswordField pwd_t2 = (PasswordField) addca_p.lookup("#cpw_2") ;
        Label label = (Label) addca_p.lookup("#label");

        btn0.setOnAction(e -> {
            String caid = caid_t.getText();
            String caname = caname_t.getText();
            String pwd = pwd_t.getText();
            String pwd2 = pwd_t2.getText();
            if(pwd.equals(pwd2)) {
                String[] spString = new String[10];
                spString[0] = "add_cashier";
                spString[1] = caid;
                spString[2] = caname;
                spString[3] = pwd;
                A.deal_s(spString);
                //System.out.println(A.result);
                pw.println(A.result);
                pw.flush();
                try {
                    if (bin.readLine().equals("true")) label.setText("   添加成功！");
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

        primaryStage.setTitle("增加客服");
        primaryStage.setScene(addca_s);
        primaryStage.show();
    }
}
