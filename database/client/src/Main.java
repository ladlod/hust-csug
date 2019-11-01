package sample;
import javafx.application.Application;
import javafx.beans.property.SimpleDoubleProperty;
import javafx.beans.property.SimpleFloatProperty;
import javafx.beans.property.SimpleIntegerProperty;
import javafx.beans.property.SimpleStringProperty;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;
import java.io.*;
import java.net.Socket;
import java.util.Observable;

public class Main extends Application {

    Integer cid;
    Integer uid;
    private final ObservableList<User> my_user_list = FXCollections.observableArrayList();
    private final ObservableList<Achievement> achievement_list = FXCollections.observableArrayList();
    @Override
    public void start(Stage primaryStage) throws Exception {
        final Socket[] socket = new Socket[1];
        PrintWriter [] pw = new PrintWriter[1];
        BufferedReader [] bin = new BufferedReader[1];
        agreement A = new agreement();
        Stage incharge_stage = new Stage();
        Stage deduct_stage = new Stage();
        Stage change_stage = new Stage();
        Stage addu_stage = new Stage();
        Stage deleteu_stage = new Stage();
        Stage change_pwd_stage = new Stage();
        Stage addc_stage = new Stage();
        Stage deletec_stage = new Stage();
        Stage addca_stage = new Stage();
        Stage deleteca_stage = new Stage();

        try {
            socket[0] = new Socket("127.0.0.1", 10010);
            pw[0] = new PrintWriter(new BufferedWriter(new OutputStreamWriter(socket[0].getOutputStream())));
            bin[0] = new BufferedReader(new InputStreamReader(socket[0].getInputStream()));
        }catch (Exception e0){
            e0.printStackTrace();
        }

        Parent root = FXMLLoader.load(getClass().getResource("sample.fxml"));
        Scene scene = new Scene(root,584,365);
        Parent b0 = FXMLLoader.load(getClass().getResource("btn_0.fxml"));
        Scene s_b0 = new Scene(b0,584,365);
        Parent b1 = FXMLLoader.load(getClass().getResource("btn_1.fxml"));
        Scene s_b1 = new Scene(b1,584,365);
        Parent b2 = FXMLLoader.load(getClass().getResource("btn_2.fxml"));
        Scene s_b2 = new Scene(b2,584,365);
        Parent b3 = FXMLLoader.load(getClass().getResource("btn_3.fxml"));
        Scene s_b3 = new Scene(b3,584,365);
        Parent manager_s = FXMLLoader.load(getClass().getResource("manager.fxml"));
        Scene m_s = new Scene(manager_s,584,365);
        Parent user_s = FXMLLoader.load(getClass().getResource("user.fxml"));
        Scene u_s = new Scene(user_s,584,365);
        Parent custom_s = FXMLLoader.load(getClass().getResource("custom.fxml"));
        Scene c_s = new Scene(custom_s,584,365);
        Parent cashier_s = FXMLLoader.load(getClass().getResource("cashier.fxml"));
        Scene ca_s = new Scene(cashier_s,584,365);
        Parent my_user_info = FXMLLoader.load(getClass().getResource("my_user_info.fxml"));
        Scene my_user_info_s = new Scene(my_user_info, 584, 365);
        Parent achievement = FXMLLoader.load(getClass().getResource("achievement.fxml"));
        Scene achievement_s = new Scene(achievement,584,365);

        Button btn0 = (Button)root.lookup("#btn_0");
        Button btn1 = (Button)root.lookup("#btn_1");
        Button btn2 = (Button)root.lookup("#btn_2");
        Button btn3 = (Button)root.lookup("#btn_3");
        Button btn00 = (Button)b0.lookup("#btn_00");
        Button btn01 = (Button)b0.lookup("#btn_01");
        Button btn10 = (Button)b1.lookup("#btn_10");
        Button btn11 = (Button)b1.lookup("#btn_11");
        Button btn20 = (Button)b2.lookup("#btn_20");
        Button btn21 = (Button)b2.lookup("#btn_21");
        Button btn30 = (Button)b3.lookup("#btn_30");
        Button btn31 = (Button)b3.lookup("#btn_31");
        Button btn000 = (Button)manager_s.lookup("#btn_000");
        Button btn001 = (Button)manager_s.lookup("#btn_001");
        Button btn002 = (Button)manager_s.lookup("#btn_002");
        Button btn003 = (Button)manager_s.lookup("#btn_003");
        Button btn004 = (Button)manager_s.lookup("#btn_004");
        Button btn005 = (Button)manager_s.lookup("#btn_005");
        Button btn100 = (Button)user_s.lookup("#btn_100");
        Button btn101 = (Button)user_s.lookup("#btn_101");
        Button btn200 = (Button)custom_s.lookup("#btn_200");
        Button btn201 = (Button)custom_s.lookup("#btn_201");
        Button btn202 = (Button)custom_s.lookup("#btn_202");
        Button btn203 = (Button)custom_s.lookup("#btn_203");
        Button btn204 = (Button)custom_s.lookup("#btn_204");
        Button btn300 = (Button)cashier_s.lookup("#btn_300");
        Button btn301 = (Button)cashier_s.lookup("#btn_301");
        Button btn302 = (Button)cashier_s.lookup("#btn_302");
        Button btn0040 = (Button)achievement_s.lookup("#btn_0040");
        Button btn3000 = (Button)my_user_info_s.lookup("#btn_3000");
        Label label0 = (Label)b0.lookup("#label_0");
        Label label1 = (Label)b1.lookup("#label_1");
        Label label2 = (Label)b2.lookup("#label_2");
        Label label3 = (Label)b3.lookup("#label_3");
        Label label10 = (Label)user_s.lookup("#label_10");
        Label label11 = (Label)user_s.lookup("#label_11");
        Label label12 = (Label)user_s.lookup("#label_12");
        Label label13 = (Label)user_s.lookup("#label_13");
        Label label14 = (Label)user_s.lookup("#label_14");
        Label label20 = (Label)custom_s.lookup("#label_20");
        Label label30 = (Label)cashier_s.lookup("#label_30");
        Label label200 = (Label)custom_s.lookup("#label_200");
        PasswordField pwd0 = (PasswordField)b0.lookup("#pwd_0");
        TextField name1 = (TextField)b1.lookup("#name_1");
        PasswordField pwd1 = (PasswordField)b1.lookup("#pwd_1");
        TextField name2 = (TextField)b2.lookup("#name_2");
        PasswordField pwd2 = (PasswordField)b2.lookup("#pwd_2");
        TextField name3 = (TextField)b3.lookup("#name_3");
        PasswordField pwd3 = (PasswordField)b3.lookup("#pwd_3");
        TableView<User> table1 = (TableView<User>)my_user_info_s.lookup("#table1");
        TableColumn idCol = new TableColumn("用户ID");        idCol.setMinWidth(100);
        TableColumn nameCol = new TableColumn("用户名");        nameCol.setMinWidth(100);
        TableColumn mealCol = new TableColumn("套餐（元）");     mealCol.setMinWidth(100);
        TableColumn moneyCol = new TableColumn("余额（元）");    moneyCol.setMinWidth(100);
        TableColumn stateCol = new TableColumn("状态");        stateCol.setMinWidth(100);
        idCol.setCellValueFactory(new PropertyValueFactory<>("uid"));
        nameCol.setCellValueFactory(new PropertyValueFactory<>("uname"));
        mealCol.setCellValueFactory(new PropertyValueFactory<>("umeal"));
        moneyCol.setCellValueFactory(new PropertyValueFactory<>("umoney"));
        stateCol.setCellValueFactory(new PropertyValueFactory<>("ustate"));
        table1.setItems(my_user_list);

        TableView<Achievement> table2 = (TableView<Achievement>)achievement_s.lookup("#table2");
        TableColumn cidCol = new TableColumn("客服ID");        cidCol.setMinWidth(100);
        TableColumn cnameCol = new TableColumn("客服名");        cnameCol.setMinWidth(100);
        TableColumn achievementCol = new TableColumn("客户数");     achievementCol.setMinWidth(100);
        cidCol.setCellValueFactory(new PropertyValueFactory<>("cid"));
        cnameCol.setCellValueFactory(new PropertyValueFactory<>("cname"));
        achievementCol.setCellValueFactory(new PropertyValueFactory<>("cconsumer"));
        table2.setItems(achievement_list);

        table1.getColumns().addAll(idCol,nameCol,mealCol,moneyCol,stateCol);
        table2.getColumns().addAll(cidCol,cnameCol,achievementCol);

        btn0.setOnAction(e->{
            primaryStage.setScene(s_b0);
        });
        btn1.setOnAction(e->{
            primaryStage.setScene(s_b1);
        });
        btn2.setOnAction(e->{
            primaryStage.setScene(s_b2);
        });
        btn3.setOnAction(e->{
            primaryStage.setScene(s_b3);
        });
        btn01.setOnAction(e->{
            pwd0.setText("");
            label0.setText("");
            primaryStage.setScene(scene);
        });
        btn11.setOnAction(e->{
            name1.setText("");

            pwd1.setText("");
            label1.setText("");
            primaryStage.setScene(scene);
        });
        btn21.setOnAction(e->{
            name2.setText("");
            pwd2.setText("");
            label2.setText("");
            primaryStage.setScene(scene);
        });
        btn31.setOnAction(e->{
            name3.setText("");
            pwd3.setText("");
            label3.setText("");
            primaryStage.setScene(scene);
        });
        btn005.setOnAction(e->{
            pwd0.setText("");
            label0.setText("");
            primaryStage.setScene(scene);
        });
        btn101.setOnAction(e->{
            name1.setText("");
            pwd1.setText("");
            label1.setText("");
            label10.setText("");label11.setText("");label12.setText("");label13.setText("");label14.setText("");
            primaryStage.setScene(scene);
        });
        btn204.setOnAction(e->{
            name2.setText("");
            pwd2.setText("");
            label2.setText("");
            label20.setText("");
            primaryStage.setScene(scene);
        });
        btn302.setOnAction(e->{
            name3.setText("");
            pwd3.setText("");
            label3.setText("");
            label30.setText("");
            primaryStage.setScene(scene);
        });
        btn0040.setOnAction(e->{
            achievement_list.clear();
            primaryStage.setScene(m_s);
        });
        btn3000.setOnAction(e->{
            my_user_list.clear();
            primaryStage.setScene(c_s);
        });

        btn00.setOnAction(e->{            //管理员login
            String mpw = pwd0.getText();
            String [] spString = new String[10];
            spString[0] = "manager_login";
            spString[1] = mpw;
            A.deal_s(spString);
            //System.out.println(A.result);
            pw[0].println(A.result);
            pw[0].flush();
            try {
                if (bin[0].readLine().equals("true"))
                    primaryStage.setScene(m_s);
                else label0.setText("密码错误！");
            }catch(Exception e0){
                e0.printStackTrace();
            }
        });
        btn10.setOnAction(e->{           //用户login
            String uname = name1.getText();
            String upw = pwd1.getText();

            String [] spString = new String[10];
            spString[0] = "user_login";
            spString[1] = uname;
            spString[2] = upw;
            A.deal_s(spString);
            //System.out.println(A.result);
            pw[0].println(A.result);
            pw[0].flush();
            try {
                spString = bin[0].readLine().split("\\s+");
                if (spString[0].equals("true")) {
                    primaryStage.setScene(u_s);
                    label10.setText(uname);
                    uid = Integer.parseInt(spString[1]);
                    label11.setText(spString[2] + " 元/月");
                    label12.setText(spString[3]);
                    label13.setText(spString[4]);
                    label14.setText(spString[5]);
                }
                else label1.setText("用户名或密码错误！");
            }catch(Exception e0){
                e0.printStackTrace();
            }
        });
        btn20.setOnAction(e->{          //客服login
            String cname = name2.getText();
            String cpw = pwd2.getText();

            String [] spString = new String[10];
            spString[0] = "custom_login";
            spString[1] = cname;
            spString[2] = cpw;
            A.deal_s(spString);
            //System.out.println(A.result);
            pw[0].println(A.result);
            pw[0].flush();
            try {
                spString = bin[0].readLine().split("\\s+");
                if (spString[0].equals("true")) {
                    cid = Integer.parseInt(spString[1]);
                    primaryStage.setScene(c_s);
                    label20.setText("客服代表 " + spString[1] + "号");
                }
                else label2.setText("用户名或密码错误！");
            }catch(Exception e0){
                e0.printStackTrace();
            }
        });
        btn30.setOnAction(e->{          //收款员login
            String caname = name3.getText();
            String capw = pwd3.getText();

            String [] spString = new String[10];
            spString[0] = "cashier_login";
            spString[1] = caname;
            spString[2] = capw;
            A.deal_s(spString);
            //System.out.println(A.result);
            pw[0].println(A.result);
            pw[0].flush();
            try {
                spString = bin[0].readLine().split("\\s+");
                //System.out.println(bin.readLine());
                if (spString[0].equals("true")) {
                    primaryStage.setScene(ca_s);
                    label30.setText("收款员 " + spString[1] + "号");
                }
                else label3.setText("用户名或密码错误！");
            }catch(Exception e0){
                e0.printStackTrace();
            }
        });

        btn000.setOnAction(e->{     //增加客服
            pop_addc_stage addc_pop = new pop_addc_stage();
            try{
                addc_pop.start(addc_stage, bin[0], pw[0]);
            }catch (Exception e0){
                e0.printStackTrace();
            }
        });
        btn001.setOnAction(e->{     //删除客服
            pop_deletec_stage deletec_pop = new pop_deletec_stage();
            try{
                deletec_pop.start(deletec_stage, bin[0], pw[0]);
            }catch (Exception e0){
                e0.printStackTrace();
            }
        });
        btn002.setOnAction(e->{     //增加收款员
            pop_addca_stage addca_pop = new pop_addca_stage();
            try{
                addca_pop.start(addca_stage, bin[0], pw[0]);
            }catch (Exception e0){
                e0.printStackTrace();
            }
        });
        btn003.setOnAction(e->{     //删除收款员
            pop_deleteca_stage deleteca_pop = new pop_deleteca_stage();
            try{
                deleteca_pop.start(deleteca_stage, bin[0], pw[0]);
            }catch (Exception e0){
                e0.printStackTrace();
            }
        });

        btn004.setOnAction(e->{     //查询客服业绩
            String [] spString = new String[10];
            spString[0] = "achievement";
            A.deal_s(spString);
            pw[0].println(A.result);
            pw[0].flush();
            try {
                spString = bin[0].readLine().split("\\s+");
                if (spString[0].equals("true")) {
                    Integer line_num = Integer.parseInt(spString[1]);
                    //System.out.println(line_num);
                    while(line_num-- > 0){
                        spString = bin[0].readLine().split("\\s+");
                        Achievement Ach = new Achievement(Integer.parseInt(spString[0]), spString[1], Integer.parseInt(spString[2]));
                        achievement_list.add(Ach);
                    }
                    primaryStage.setScene(achievement_s);
                }
                else label200.setText("无客服！");
            }catch(Exception e0){
                e0.printStackTrace();
            }
        });

        btn100.setOnAction(e->{   //修改密码
            pop_cpwd_stage cpwd_pop = new pop_cpwd_stage();
            try{
                cpwd_pop.start(change_pwd_stage, uid, bin[0], pw[0]);
            }catch (Exception e0){
                e0.printStackTrace();
            }
        });

        btn200.setOnAction(e->{   //开户
            pop_addu_stage addu_pop = new pop_addu_stage();
            try{
                addu_pop.start(addu_stage, cid, bin[0], pw[0]);
            }catch (Exception e0){
                e0.printStackTrace();
            }
        });
        btn201.setOnAction(e->{   //销户
            pop_deleteu_stage deleteu_pop = new pop_deleteu_stage();
            try{
                deleteu_pop.start(deleteu_stage, bin[0], pw[0]);
            }catch (Exception e0){
                e0.printStackTrace();
            }
        });
        btn202.setOnAction(e->{    //修改用户套餐
            pop_change_stage change_pop = new pop_change_stage();
            try {
                change_pop.start(change_stage, bin[0], pw[0]);
            }catch (Exception e0){
                e0.printStackTrace();
            }
        });
        btn203.setOnAction(e->{   //查询我服务的用户信息
            String [] spString = new String[10];
            spString[0] = "select_my_user";
            spString[1] = cid.toString();
            A.deal_s(spString);
            pw[0].println(A.result);
            pw[0].flush();
            try {
                spString = bin[0].readLine().split("\\s+");
                if (spString[0].equals("true")) {
                    Integer line_num = Integer.parseInt(spString[1]);
                    while(line_num-- > 0){
                        spString = bin[0].readLine().split("\\s+");
                        User U = new User(Integer.parseInt(spString[0]), spString[1], Float.parseFloat(spString[2]),Float.parseFloat(spString[3]),spString[4]);
                        my_user_list.add(U);
                    }
                    //System.out.println(my_user_list.size());
                    primaryStage.setScene(my_user_info_s);
                }
                else label200.setText("无用户！");
            }catch(Exception e0){
                e0.printStackTrace();
            }
        });

        btn300.setOnAction(e->{  //收款员缴费
            pop_incharge_stage incharge_pop = new pop_incharge_stage();
            try {
                incharge_pop.start(incharge_stage, bin[0], pw[0]);
            }catch (Exception e0){
                e0.printStackTrace();
            }
        });

        btn301.setOnAction(e->{  //收款员扣费
            pop_deduct_stage deduct_pop = new pop_deduct_stage();
            try {
                deduct_pop.start(deduct_stage, bin[0], pw[0]);
            }catch (Exception e0){
                e0.printStackTrace();
            }
        });

        primaryStage.setOnCloseRequest(e->{  //关闭窗口
            try {
                bin[0].close();
                pw[0].close();
                socket[0].close();
            }catch (Exception e0){
                e0.printStackTrace();
            }
        });

        primaryStage.setTitle("电信收费管理系统");
        primaryStage.setScene(scene);
        primaryStage.show();
    }

    public static void main(String[] args) {
        launch(args);
    }
}