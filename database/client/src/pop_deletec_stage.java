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
public class pop_deletec_stage {
    public void start(Stage primaryStage, BufferedReader bin, PrintWriter pw) throws Exception{
        agreement A = new agreement();

        Parent deletec_p = FXMLLoader.load(getClass().getResource("deletec.fxml"));
        Scene deletec_s = new Scene(deletec_p);
        Button btn0 = (Button) deletec_p.lookup("#btn_0");
        Button btn1 = (Button) deletec_p.lookup("#btn_1");
        TextField cid_t = (TextField) deletec_p.lookup("#id");
        Label label = (Label) deletec_p.lookup("#label");

        btn0.setOnAction(e -> {
            String cid = cid_t.getText();
            String[] spString = new String[10];
            spString[0] = "delete_custom";
            spString[1] = cid;
            A.deal_s(spString);
            pw.println(A.result);
            pw.flush();
            try {
                if (bin.readLine().equals("true")) label.setText("   删除成功！");
                else label.setText("id不存在或无法删除！");
            } catch (Exception e0) {
                e0.printStackTrace();
            }
        });

        btn1.setOnAction(e -> primaryStage.close());

        primaryStage.setTitle("删除客服");
        primaryStage.setScene(deletec_s);
        primaryStage.show();
    }
}
