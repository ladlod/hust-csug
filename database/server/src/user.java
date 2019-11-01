import java.sql.*;

/**
 * Created by ladlod on 19-5-25.
 * 用户类
 */
public class user {
    Integer uid;
    String uname;
    Float meal;
    Float money;
    String state;
    Integer cid;
    String upw;

    boolean user_login(String pwd){ //用户登录
        try{
            Connection connection = DriverManager.getConnection("jdbc:mysql://localhost/Telecom", "root", "tzw@112358");
            PreparedStatement statement = connection.prepareStatement("select * from User where uname=" + "\"" + this.uname + "\"");
            ResultSet resultset = statement.executeQuery();
            if(resultset.next()){
                this.uid = resultset.getInt(1);
                this.meal = resultset.getFloat(3);
                this.money = resultset.getFloat(4);
                this.state = resultset.getString(5);
                this.cid = resultset.getInt(6);
                this.upw = resultset.getString(7);
                //System.out.println(this.upw);
                if(this.upw.equals(pwd))
                    return true;
            }
        }catch (Exception e){
            e.printStackTrace();
        }

        return false;
    }

    static boolean change_pwd(Integer uid, String pwd){   //修改密码
        try {
            Connection connection = DriverManager.getConnection("jdbc:mysql://localhost/Telecom", "root", "tzw@112358");
            Statement statement = connection.createStatement();
            statement.executeUpdate("update User set upw = " + "\"" + pwd + "\"" + " where uid=" + uid);
        }catch (Exception e){
            e.printStackTrace();
        }

        return true;
    }


}
