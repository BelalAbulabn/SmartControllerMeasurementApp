package gui;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.PrintWriter;

import static gui.Windo.*;
    /*
 Diese Klasse ist verantwortlich für die Steuerung der PWM-Vollbrückensignale
für den zweiten Vollbrückenschaltkreis.
    */

public class Start_PWM_2_Full_bridge_Button implements ActionListener {
    @Override
    public void actionPerformed(ActionEvent e) {
        if(Windo.Start_PWM_2_Full_bridge.getText()=="Start(voll)"){
            try {
                PrintWriter output = new PrintWriter(chosenPort.getOutputStream());
                output.print("2/"+PWMCaluation());
                System.out.println("2/"+PWMCaluation()+"\r" );
                Windo.Start_PWM_2_Full_bridge.setText("Stop");output.flush();
                output.close();
                Windo.Start_PWM_2_Full_bridge.setText("Stop");//Scann();
                Windo.Start_PWM_2_Full_bridge.setText("Stop");
            }catch (Exception NullPointerException){
                JOptionPane.showMessageDialog(null, "Bitte überprüfung Sie die Verbindung mit dem richrigen Port   \n" );
            }

        }			else if(Windo.Start_PWM_2_Full_bridge.getText()=="Stop"){
            PrintWriter output = new PrintWriter(chosenPort.getOutputStream());output.print("dis"+ "\r");
            output.flush();
            output.close();
            Windo.Start_PWM_2_Full_bridge.setText("Start(voll)");
        }

    }

    String PWMCaluation() {
        //168=1us
        String Massg = null;
        try {


            double frequenz_pwm_1Value = Double.parseDouble(Frequenz_PWM_2.getText());
            double duty_cycle_pwm_1_Value = Duty_Cycle_PWM_2;
            double peroid_pwm_1Value = 1 / frequenz_pwm_1Value;

            int peroid_pwm_1Valuetimer = (int) (peroid_pwm_1Value * 168 *33* 10);
            //int duty_cycle_pwm_1_percent = (int) (((duty_cycle_pwm_1_Value) / 100) * peroid_pwm_1Valuetimer);
            double duty_cycle_pwm_1_percent =  ((duty_cycle_pwm_1_Value)  * peroid_pwm_1Valuetimer);
            System.out.println(duty_cycle_pwm_1_percent);
            Massg = "2/" + String.valueOf(peroid_pwm_1Valuetimer) + "/" + String.valueOf((int)duty_cycle_pwm_1_percent);
            return Massg;

        } catch (Exception NumberFormatException) {
            JOptionPane.showMessageDialog(null, "Bitte Geben Sie die Frequenz und Duty Cycle  \n" );
        }
        return Massg;
    }

}

