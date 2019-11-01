package sample;

import javafx.beans.property.SimpleIntegerProperty;
import javafx.beans.property.SimpleStringProperty;

/**
 * Created by ladlod on 19-5-29.
 */
public class Achievement {
    private final SimpleIntegerProperty cid;
    private final SimpleStringProperty cname;
    private final SimpleIntegerProperty cconsumer;

    Achievement(Integer id, String name, Integer consumer){
        this.cid = new SimpleIntegerProperty(id);
        this.cname = new SimpleStringProperty(name);
        this.cconsumer = new SimpleIntegerProperty(consumer);
    }

    public void setCid(Integer id){
        cid.set(id);
    }
    public void setCname(String name){
        cname.set(name);
    }
    public void setCconsumer(Integer consumer){
        cconsumer.set(consumer);
    }

    public Integer getCid(){
        return cid.get();
    }
    public String getCname(){
        return cname.get();
    }
    public Integer getCconsumer(){
        return cconsumer.get();
    }
}
