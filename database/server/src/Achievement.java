/**
 * Created by ladlod on 19-5-29.
 */
public class Achievement implements Comparable<Achievement>{
    Integer cid;
    String cname;
    Integer achievement;

    public int compareTo(Achievement a){
        if(this.cid < a.cid) return 1;
        return -1;
    }
}
