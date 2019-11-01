/**
 * Created by ladlod on 19-5-25.
 * 客服代表类
 */
import java.sql.*;
import java.util.List;

public class custom {
    Integer cid;
    String cname;
    String cpw;
    Integer length;

    boolean custom_login(String pwd){
        try{
            Connection connection = DriverManager.getConnection("jdbc:mysql://localhost/Telecom", "root", "tzw@112358");
            PreparedStatement statement = connection.prepareStatement("select * from Custom where cname=" + "\"" + this.cname + "\"");
            ResultSet resultset = statement.executeQuery();
            if(resultset.next()){
                this.cid = resultset.getInt(1);
                this.cpw = resultset.getString(3);
                if(this.cpw.equals(pwd))
                    return true;
            }
        }catch (Exception e){
            e.printStackTrace();
        }

        return false;
    }

    static boolean add_user(Integer uid,String uname, Float meal, Float money, String state, Integer cid, String upw){ //为用户开户
        try{
            Connection connection = DriverManager.getConnection("jdbc:mysql://localhost/Telecom", "root", "tzw@112358");
            PreparedStatement statement = connection.prepareStatement("select * from User where uid=" + uid);
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
            statement.executeUpdate("insert into User" + " values(" + uid + ",\"" + uname + "\"," + meal + "," + money + ",\"" + state + "\"," + cid + ",\"" + upw + "\")");
        }catch (Exception e){
            e.printStackTrace();
        }

        return true;
    }

    static boolean remove_user(Integer uid){   //为用户销户
        try{
            Connection connection = DriverManager.getConnection("jdbc:mysql://localhost/Telecom", "root", "tzw@112358");
            PreparedStatement statement = connection.prepareStatement("select * from User where uid=" + uid);
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
            statement.executeUpdate("delete from User where uid=" + uid);
        }catch (Exception e){
            e.printStackTrace();
        }
        return true;
    }

    static boolean change_user_meal(Integer uid, Float meal){   //为用户修改套餐
        try{
            Connection connection = DriverManager.getConnection("jdbc:mysql://localhost/Telecom", "root", "tzw@112358");
            PreparedStatement statement = connection.prepareStatement("select * from User where uid=" + uid);
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
            statement.executeUpdate("update User set meal = " + meal + " where uid=" + uid);
        }catch (Exception e){
            e.printStackTrace();
        }

        return true;
    }

    boolean select_my_user(List<user> U){ //查询我的用户信息
        try {
            Connection connection = DriverManager.getConnection("jdbc:mysql://localhost/Telecom", "root", "tzw@112358");
            PreparedStatement statement = connection.prepareStatement("select * from User where cid=" + cid + " order by uid desc");
            ResultSet resultset = statement.executeQuery();
            while (resultset.next()) {
                user userl = new user();
                userl.uid = resultset.getInt(1);
                userl.uname = resultset.getString(2);
                userl.meal = resultset.getFloat(3);
                userl.money = resultset.getFloat(4);
                userl.state = resultset.getString(5);
                userl.cid = resultset.getInt(6);
                U.add(userl);
            }
            length = U.size();
        }catch (Exception e){
            e.printStackTrace();
        }

        return true;
    }
}
