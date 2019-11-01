/**
 * Created by ladlod on 19-5-25.
 */
package sample;

public class agreement {
    String result = null;
    void deal_s(String [] spString){
        int i = 1;
        this.result = spString[0];
        while(spString[i]!=null) {
            this.result = this.result + " " + spString[i];
            i++;
        }
        //System.out.println(this.result);
    }
}

