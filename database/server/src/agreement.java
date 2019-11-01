/**
 * Created by ladlod on 19-5-25.
 */
public class agreement {
    String method;
    Integer id;
    String name;
    Float meal;
    Float money;
    String state;
    Integer cid;
    String pwd;
    String result = null;
    void deal_out(String [] spString){
        int i = 1;
        this.result = spString[0];
        while(spString[i]!=null) {
            this.result = this.result + " " + spString[i];
            i++;
        }
        //System.out.println(this.result);
    }

    void deal_in(String s) {
        String[] spString = s.split("\\s+");
        this.method = spString[0];
        if (method.equals("manager_login")) this.pwd = spString[1];
        else if (method.equals("user_login") || method.equals("custom_login") || method.equals("cashier_login")) {
            this.name = spString[1];
            this.pwd = spString[2];
        }
        else if (method.equals("add_custom") || method.equals("add_cashier")) {
            this.id = Integer.parseInt(spString[1]);
            this.name = spString[2];
            this.pwd = spString[3];
        }
        else if(method.equals("delete_custom") || method.equals("delete_cashier") || method.equals("delete_user") || method.equals("select_my_user")){
            this.id = Integer.parseInt(spString[1]);
        }
        else if(method.equals("change_password")){
            this.id = Integer.parseInt(spString[1]);
            this.pwd = spString[2];
        }
        else if(method.equals("add_user")){
            this.id = Integer.parseInt(spString[1]);
            this.name = spString[2];
            this.meal = Float.parseFloat(spString[3]);
            this.money = Float.parseFloat(spString[4]);
            this.state = spString[5];
            this.cid = Integer.parseInt(spString[6]);
            this.pwd = spString[7];
        }
        else if(method.equals("change_user_meal")){
            this.id = Integer.parseInt(spString[1]);
            this.meal = Float.parseFloat(spString[2]);
        }
        else if(method.equals("incharge") || method.equals("deduct")){
            this.id = Integer.parseInt(spString[1]);
            this.money = Float.parseFloat(spString[2]);
        }
    }

}
