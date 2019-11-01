import java.sql.*;

/**
 * Created by ladlod on 19-5-25.
 * 收款员类
 */
public class cashier {
    Integer caid;
    String caname;
    String capw;

    boolean cashier_login(String pwd){
        try{
            Connection connection = DriverManager.getConnection("jdbc:mysql://localhost/Telecom", "root", "tzw@112358");
            PreparedStatement statement = connection.prepareStatement("select * from Cashier where caname=" + "\"" + this.caname + "\"");
            ResultSet resultset = statement.executeQuery();
            if(resultset.next()){
                this.caid = resultset.getInt(1);
                this.capw = resultset.getString(3);
                if(this.capw.equals(pwd))
                    return true;
            }
        }catch (Exception e){
            e.printStackTrace();
        }

        return false;
    }

    boolean incharge(Integer uid, Float money){ //为用户缴费
        Float money_n = new Float(0);
        try{
            Connection connection = DriverManager.getConnection("jdbc:mysql://localhost/Telecom", "root", "tzw@112358");
            PreparedStatement pstatement = connection.prepareStatement("select * from User where uid=" + uid);
            ResultSet resultset = pstatement.executeQuery();
            if(resultset.next()){
                money_n = resultset.getFloat(4);
                money_n += money;
                Statement statement = connection.createStatement();
                if(money_n >= 0)
                    statement.executeUpdate("update User set state = \"正常\" where uid = " + uid);
                statement.executeUpdate("update User set money = " + money_n + " where uid=" + uid);
                return true;
            }

        }catch(Exception e){
            e.printStackTrace();
        }

        return false;
    }

    boolean deduct(Integer uid, Float money){ //为用户扣费
        Float money_n = new Float(0);
        try{
            Connection connection = DriverManager.getConnection("jdbc:mysql://localhost/Telecom", "root", "tzw@112358");
            PreparedStatement pstatement = connection.prepareStatement("select * from User where uid=" + uid);
            ResultSet resultset = pstatement.executeQuery();
            if(resultset.next()){
                money_n = resultset.getFloat(4);
                money_n -= money;
                Statement statement = connection.createStatement();
                if(money_n < 0)
                    statement.executeUpdate("update User set state = \"欠费\" where uid = " + uid);
                statement.executeUpdate("update User set money = " + money_n + " where uid=" + uid);
                return true;
            }

        }catch(Exception e){
            e.printStackTrace();
        }

        return false;
    }
}
