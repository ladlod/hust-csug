package sample;

import javafx.beans.property.SimpleFloatProperty;
import javafx.beans.property.SimpleIntegerProperty;
import javafx.beans.property.SimpleStringProperty;

/**
 * Created by ladlod on 19-5-29.
 */
public class User {
    private final SimpleIntegerProperty uid;
    private final SimpleStringProperty uname;
    private final SimpleFloatProperty umeal;
    private final SimpleFloatProperty umoney;
    private final SimpleStringProperty ustate;

    User(Integer id, String name, Float meal, Float money, String state){
        this.uid = new SimpleIntegerProperty(id);
        this.uname = new SimpleStringProperty(name);
        this.umeal = new SimpleFloatProperty(meal);
        this.umoney = new SimpleFloatProperty(money);
        this.ustate = new SimpleStringProperty(state);
    }
    public void setUid(Integer id){
        uid.set(id);
    }
    public Integer getUid(){
        return uid.get();
    }
    public void setUname(String name){
        uname.set(name);
    }
    public String getUname(){
        return uname.get();
    }
    public void setUmeal(Float meal){
        umeal.set(meal);
    }
    public Float getUmeal(){
        return umeal.get();
    }
    public void setUmoney(Float money){
        umoney.set(money);
    }
    public Float getUmoney(){
        return umoney.get();
    }
    public void setUstate(String state){
        ustate.set(state);
    }
    public String getUstate(){
        return ustate.get();
    }
}
