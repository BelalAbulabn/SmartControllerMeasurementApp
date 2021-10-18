package JASON;

import com.github.cliftonlabs.json_simple.JsonObject;

public class Save_PWM_SonClass extends Profilesettings_FatherClass {
    String DutyCycle;
    public Save_PWM_SonClass(String uon, String ton, String tin, String tout, String tpeaK, String frequnzy, String number) {
        super(uon, ton, tin, tout, tpeaK, frequnzy, number);

    }

    public String getDutyCycle() {
        return DutyCycle;
    }

    public void setDutyCycle(String dutyCycle) {
        DutyCycle = dutyCycle;
    }

    public Object pwm(String output)
    {
        JsonObject jason = new JsonObject();
        jason.put("PWM", output);
        JsonObject output1 = new JsonObject();
        output1.put("Frquency",getFrequnzy());
        output1.put("Duty Cycle",getFrequnzy());
        return jason;
    }
}
