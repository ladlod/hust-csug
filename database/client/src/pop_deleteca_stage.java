package sample;

import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.stage.Stage;

import java.io.BufferedReader;
import java.io.PrintWriter;

/**
 * Created by ladlod on 19-5-27.
 */
public class pop_deleteca_stage {
    public void start(Stage primaryStage, BufferedReader bin, PrintWriter pw) throws Exception{
        agreement A = new agreement();

        Parent deleteca_p = FXMLLoader.load(getClass().getResource("deleteca.fxml"));
        Scene deleteca_s = new Scene(deleteca_p);
        Button btn0 = (Button) deleteca_p.lookup("#btn_0");
        Button btn1 = (Button) deleteca_p.lookup("#btn_1");
        TextField caid_t = (TextField) deleteca_p.lookup("#id");
        Label label = (Label) deleteca_p.lookup("#label");

        btn0.setOnAction(e -> {
            String caid = caid_t.getText();
            String[] spString = new String[10];
            spString[0] = "delete_cashier";
            spString[1] = caid;
            A.deal_s(spString);
            pw.println(A.result);
            pw.flush();
            try {
                if (bin.readLine().equals("true")) label.setText("删除成功！");
                else label.setText("id不存在！");
            } catch (Exception e0) {
                e0.printStackTrace();
            }
        });

        btn1.setOnAction(e -> primaryStage.close());

        primaryStage.setTitle("删除客服");
        primaryStage.setScene(deleteca_s);
        primaryStage.show();
    }
}
