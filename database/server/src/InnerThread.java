/**
 * Created by ladlod on 19-5-28.
 */

import java.io.*;
import java.net.*;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
public class InnerThread implements Runnable{
    private Socket socket;
    public InnerThread(Socket socket){
        this.socket = socket;
    }
    public void run(){
        BufferedReader bin = null;
        PrintWriter os = null;
        try {
            bin = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            os = new PrintWriter(new BufferedWriter(new OutputStreamWriter(socket.getOutputStream())), true);
        } catch (Exception e) {
            e.printStackTrace();
        }

        while (true) {
            agreement A = new agreement();
            String commond = null;

            try {
                commond = bin.readLine();
            } catch (Exception e) {
                e.printStackTrace();
            }

            if (commond == null) {
                break;
            }
            System.out.println(commond);
            A.deal_in(commond);

            //管理员登录请求
            if (A.method.equals("manager_login")) {
                manager M = new manager();
                boolean T = M.manager_login(A.pwd);
                if (T == true) os.println("true");
                else os.println("false");
                os.flush();
            }
            //用户登录请求
            else if (A.method.equals("user_login")) {
                user U = new user();
                U.uname = A.name;
                boolean T = U.user_login(A.pwd);
                if (T == true) {
                    String[] spString = new String[10];
                    spString[0] = "true";
                    spString[1] = U.uid.toString();
                    spString[2] = U.meal.toString();
                    spString[3] = U.money.toString();
                    spString[4] = U.state;
                    spString[5] = U.cid.toString();
                    A.deal_out(spString);
                    os.println(A.result);
                } else os.println("false");
                os.flush();
            }
            //客服登录请求
            else if (A.method.equals("custom_login")) {
                custom C = new custom();
                C.cname = A.name;
                boolean T = C.custom_login(A.pwd);
                if (T == true) {
                    String[] spString = new String[10];
                    spString[0] = "true";
                    spString[1] = C.cid.toString();
                    A.deal_out(spString);
                    os.println(A.result);
                } else os.println("false");
                os.flush();
            }
            //收款员登录请求
            else if (A.method.equals("cashier_login")) {
                cashier CA = new cashier();
                CA.caname = A.name;
                boolean T = CA.cashier_login(A.pwd);
                if (T == true) {
                    String[] spString = new String[10];
                    spString[0] = "true";
                    spString[1] = CA.caid.toString();
                    A.deal_out(spString);
                    os.println(A.result);
                } else os.println("false");
                os.flush();
            }
            //增加客服请求
            else if (A.method.equals("add_custom")) {
                manager M = new manager();
                boolean T = M.add_custom(A.id,A.name,A.pwd);
                if (T == true) {
                    os.println("true");
                } else os.println("false");
                os.flush();
            }
            //删除客服请求
            else if (A.method.equals("delete_custom")) {
                manager M = new manager();
                boolean T = M.remove_custom(A.id);
                if (T == true) {
                    os.println("true");
                } else os.println("false");
                os.flush();
            }
            //增加收款员请求
            else if (A.method.equals("add_cashier")) {
                manager M = new manager();
                boolean T = M.add_cashier(A.id,A.name,A.pwd);
                if (T == true) {
                    os.println("true");
                } else os.println("false");
                os.flush();
            }
            //删除收款员请求
            else if (A.method.equals("delete_cashier")) {
                manager M = new manager();
                boolean T = M.remove_cashier(A.id);
                if (T == true) {
                    os.println("true");
                } else os.println("false");
                os.flush();
            }
            //统计客服业绩请求
            else if(A.method.equals("achievement")){
                manager M = new manager();

                List<Achievement> Ach = new ArrayList<>();
                boolean T = M.achievement(Ach);
                if(T == true) {
                    String[] spString = new String[1024];
                    spString[0] = "true";
                    spString[1] = M.length.toString();
                    A.deal_out(spString);
                    os.println(A.result);
                    os.flush();
                    int i = M.length;
                    while(i-- > 0){
                        spString[0] = Ach.get(i).cid.toString();
                        spString[1] = Ach.get(i).cname;
                        spString[2] = Ach.get(i).achievement.toString();
                        A.deal_out(spString);
                        os.println(A.result);
                        os.flush();
                    }
                }
                else{
                    os.println("false");
                    os.flush();
                }
            }
            //修改密码请求
            else if(A.method.equals("change_password")){
                user U = new user();
                U.change_pwd(A.id, A.pwd);
                os.println("true");
                os.flush();
            }
            //开户请求
            else if (A.method.equals("add_user")) {
                custom C = new custom();
                boolean T = C.add_user(A.id, A.name, A.meal, A.money, A.state, A.cid, A.pwd);
                if (T == true) {
                    os.println("true");
                } else os.println("false");
                os.flush();
            }
            //销户请求
            else if (A.method.equals("delete_user")) {
                custom C = new custom();
                boolean T = C.remove_user(A.id);
                if (T == true) {
                    os.println("true");
                } else os.println("false");
                os.flush();
            }
            //修改套餐请求
            else if (A.method.equals("change_user_meal")) {
                custom C = new custom();
                boolean T = C.change_user_meal(A.id, A.meal);
                if (T == true) {
                    os.println("true");
                } else os.println("false");
                os.flush();
            }
            //查询我的客户请求
            else if(A.method.equals("select_my_user")){
                custom C = new custom();
                C.cid = A.id;
                List<user> U = new ArrayList<>();
                boolean T = C.select_my_user(U);
                if(T == true) {
                    String[] spString = new String[1024];
                    spString[0] = "true";
                    spString[1] = C.length.toString();
                    A.deal_out(spString);
                    os.println(A.result);
                    os.flush();
                    int i = C.length;
                    while(i-- > 0){
                        spString[0] = U.get(i).uid.toString();
                        spString[1] = U.get(i).uname;
                        spString[2] = U.get(i).meal.toString();
                        spString[3] = U.get(i).money.toString();
                        spString[4] = U.get(i).state;
                        A.deal_out(spString);
                        os.println(A.result);
                        os.flush();
                    }
                }
                else{
                    os.println("false");
                    os.flush();
                }
            }
            //缴费请求
            else if (A.method.equals("incharge")) {
                cashier CA = new cashier();
                boolean T = CA.incharge(A.id, A.money);
                if (T == true) {
                    os.println("true");
                } else os.println("false");
                os.flush();
            }
            //扣费请求
            else if (A.method.equals("deduct")) {
                cashier CA = new cashier();
                boolean T = CA.deduct(A.id, A.money);
                if (T == true) {
                    os.println("true");
                } else os.println("false");
                os.flush();
            }
        }
        try{
            os.close();
            bin.close();
            socket.close();
        }catch (Exception e){
            e.printStackTrace();
        }
    }
}
