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
public class pop_change_stage {
    public void start(Stage primaryStage, BufferedReader bin, PrintWriter pw) throws Exception{
        agreement A = new agreement();

        Parent change_p = FXMLLoader.load(getClass().getResource("change.fxml"));
        Scene change_s = new Scene(change_p);
        Button btn0 = (Button)change_p.lookup("#btn_0");
        Button btn1 = (Button)change_p.lookup("#btn_1");
        TextField id = (TextField)change_p.lookup("#id");
        TextField meal = (TextField)change_p.lookup("#meal");
        Label label = (Label)change_p.lookup("#label");

        btn0.setOnAction(e->{
            String uid = id.getText();
            String umeal = meal.getText();
            String [] spString = new String[10];
            spString[0] = "change_user_meal";
            spString[1] = uid;
            spString[2] = umeal;
            A.deal_s(spString);
            //System.out.println(A.result);
            pw.println(A.result);
            pw.flush();
            try {
                if (bin.readLine().equals("true")) label.setText("修改成功！");
                else label.setText("用户不存在！");
            }catch(Exception e0){
                e0.printStackTrace();
            }

        });

        btn1.setOnAction(e-> primaryStage.close());

        primaryStage.setTitle("修改套餐");
        primaryStage.setScene(change_s);
        primaryStage.show();
    }
}
