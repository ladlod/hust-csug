import java.sql.*;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

/**
 * Created by ladlod on 19-5-25.
 * 管理员类
 */
public class manager {

    Integer length;
    static boolean manager_login(String pwd){
        if(pwd.equals("tzw987654321")){
            return true;
        }
        return false;
    }

    static boolean add_custom(Integer cid, String cname, String cpw){ //增加客服
        try{
            Connection connection = DriverManager.getConnection("jdbc:mysql://localhost/Telecom", "root", "tzw@112358");
            PreparedStatement statement = connection.prepareStatement("select * from Custom where cid=" + cid);
            ResultSet resultset = statement.executeQuery();
            if(resultset.next()){
                return false;
            }
        }catch (Exception e){
            e.printStackTrace();
        }
        try {
            Connection connection = DriverManager.getConnection("jdbc:mysql://localhost/Telecom", "root", "tzw@112358");
            Statement statement = connection.createStatement();
            statement.executeUpdate("insert into Custom" + " values(" + cid + ",\"" + cname + "\",\""+ cpw + "\")");
        }catch (Exception e){
            e.printStackTrace();
        }
        return true;
    }

    static boolean remove_custom(Integer cid){ //删除客服
        try{
            Connection connection = DriverManager.getConnection("jdbc:mysql://localhost/Telecom", "root", "tzw@112358");
            PreparedStatement statement = connection.prepareStatement("select * from Custom where cid=" + cid);
            ResultSet resultset = statement.executeQuery();
            if(!resultset.next()){
                return false;
            }
        }catch (Exception e){
            e.printStackTrace();
        }
        try{
            Connection connection = DriverManager.getConnection("jdbc:mysql://localhost/Telecom", "root", "tzw@112358");
            PreparedStatement statement = connection.prepareStatement("select * from User where cid=" + cid);
            ResultSet resultset = statement.executeQuery();
            if(resultset.next()){
                return false;
            }
        }catch (Exception e){
            e.printStackTrace();
        }
        try {
            Connection connection = DriverManager.getConnection("jdbc:mysql://localhost/Telecom", "root", "tzw@112358");
            Statement statement = connection.createStatement();
            statement.executeUpdate("delete from Custom where cid=" + cid);
        }catch (Exception e){
            e.printStackTrace();
        }
        return true;
    }

    static boolean add_cashier(Integer caid, String caname, String capw){ //增加收款员
        try{
            Connection connection = DriverManager.getConnection("jdbc:mysql://localhost/Telecom", "root", "tzw@112358");
            PreparedStatement statement = connection.prepareStatement("select * from Cashier where caid=" + caid);
            ResultSet resultset = statement.executeQuery();
            if(resultset.next()){
                return false;
            }
        }catch (Exception e){
            e.printStackTrace();
        }
        try {
            Connection connection = DriverManager.getConnection("jdbc:mysql://localhost/Telecom", "root", "tzw@112358");
            Statement statement = connection.createStatement();
            statement.executeUpdate("insert into Cashier" + " values(" + caid + ",\"" + caname + "\",\"" + capw + "\")");
        }catch (Exception e){
            e.printStackTrace();
        }
        return true;
    }
    static boolean remove_cashier(Integer caid){ //删除收款员
        try{
            Connection connection = DriverManager.getConnection("jdbc:mysql://localhost/Telecom", "root", "tzw@112358");
            PreparedStatement statement = connection.prepareStatement("select * from Cashier where caid=" + caid);
            ResultSet resultset = statement.executeQuery();
            if(!resultset.next()){
                return false;
            }
        }catch (Exception e){
            e.printStackTrace();
        }
        try {
            Connection connection = DriverManager.getConnection("jdbc:mysql://localhost/Telecom", "root", "tzw@112358");
            Statement statement = connection.createStatement();
            statement.executeUpdate("delete from Cashier where caid=" + caid);
        }catch (Exception e){
            e.printStackTrace();
        }
        return true;
    }
    boolean achievement(List<Achievement> A){
        try {
            Connection connection = DriverManager.getConnection("jdbc:mysql://localhost/Telecom", "root", "tzw@112358");
            PreparedStatement statement = connection.prepareStatement("select Custom.cid,cname,count(uid) from User,Custom where User.cid=Custom.cid group by cid order by cid desc");
            ResultSet resultset = statement.executeQuery();
            while (resultset.next()) {
                Achievement al = new Achievement();
                al.cid = resultset.getInt(1);
                al.cname = resultset.getString(2);
                al.achievement = resultset.getInt(3);

                A.add(al);
            }
        }catch (Exception e){
            e.printStackTrace();
        }
        try {
            Connection connection = DriverManager.getConnection("jdbc:mysql://localhost/Telecom", "root", "tzw@112358");
            PreparedStatement statement = connection.prepareStatement("select cid,cname from Custom where cid not in(select Custom.cid from User,Custom where Custom.cid = User.cid group by cid)");
            ResultSet resultset = statement.executeQuery();
            while (resultset.next()) {
                Achievement al = new Achievement();
                al.cid = resultset.getInt(1);
                al.cname = resultset.getString(2);
                al.achievement = 0;
                A.add(al);
            }
            A.sort(Comparator.naturalOrder());
        }catch(Exception e){
            e.printStackTrace();
        }
        length = A.size();

        return true;
    }
}
