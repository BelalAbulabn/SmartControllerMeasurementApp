package JASON;

import com.github.cliftonlabs.json_simple.JsonObject;
/*
Diese Klasse ist für die Speicherung der Profileinstellungen in einem JASON-Formular zuständig.
 */
public class Profilesettings_FatherClass {
   private String uon;
    private String Ton;
    private String Tin;
    private String Tout;
    private  String TpeaK;
    private String Frequnzy;
    private  String number;

    public Profilesettings_FatherClass(String uon, String ton, String tin, String tout, String tpeaK, String frequnzy, String number) {
        JsonObject output1 = new JsonObject();
        this.uon = uon;
        this.Ton = ton;
        this.Tin = tin;
        this.Tout = tout;
        this.TpeaK = tpeaK;
        this.Frequnzy = frequnzy;
        this.number = number;
    }

    public String getUon() {
        return uon;
    }

    public void setUon(String uon) {
        this.uon = uon;
    }

    public String getTon() {
        return Ton;
    }

    public void setTon(String ton) {
        Ton = ton;
    }

    public String getTin() {
        return Tin;
    }

    public void setTin(String tin) {
        Tin = tin;
    }

    public String getTout() {
        return Tout;
    }

    public void setTout(String tout) {
        Tout = tout;
    }

    public String getTpeaK() {
        return TpeaK;
    }

    public void setTpeaK(String tpeaK) {
        TpeaK = tpeaK;
    }

    public String getFrequnzy() {
        return Frequnzy;
    }

    public void setFrequnzy(String frequnzy) {
        Frequnzy = frequnzy;
    }

    public String getNumber() {
        return number;
    }

    public void setNumber(String number) {
        this.number = number;
    }
    public Object json(String output)
    {
        JsonObject json = new JsonObject();
        json.put("output",output);

        JsonObject output1 = new JsonObject();
        output1.put("volte Value",getUon());
        output1.put("Time on",getTon());
        output1.put("Time in ",getTin());
        output1.put("Time out",getTout());
        output1.put("Time Peak",getTpeaK());
        output1.put("Frquency",getFrequnzy());
        output1.put("Numberofpulsess",getNumber());
        json.put(output,output1);
        return json;

    }


}
