package gui;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import static gui.Windo.*;

class Duty_Cycle_2_ComboBox implements ActionListener {
    /*
    Diese Klasse ist für die Auswahl des Duty Cycle der PWM und die Berechnung der Timerwerte des Mikrocontrollers zuständig.
    Für die zweite  Vollbrückenschaltung
     */
    @Override
    public void actionPerformed(ActionEvent e) {


        if(DutyCycle_2.getSelectedItem()=="5%")
        {
            Duty_Cycle_PWM_2=0.05;

        }
        if(DutyCycle_2.getSelectedItem()=="10%")
        {
            Duty_Cycle_PWM_2=0.1;

        }
        if(DutyCycle_2.getSelectedItem()=="20%")
        {
            Duty_Cycle_PWM_2=0.2;

        }
        if(DutyCycle_2.getSelectedItem()=="30%")
        {
            Duty_Cycle_PWM_2=0.3;

        }
        if(DutyCycle_2.getSelectedItem()=="40%")
        {
            Duty_Cycle_PWM_2=0.4;

        }
        if(DutyCycle_2.getSelectedItem()=="50%")
        {
            Duty_Cycle_PWM_2=0.5;

        }
        if(DutyCycle_2.getSelectedItem()=="60%")
        {
            Duty_Cycle_PWM_2=0.6;

        }
        if(DutyCycle_2.getSelectedItem()=="70%")
        {
            Duty_Cycle_PWM_2=0.7;


        }
        if(DutyCycle_2.getSelectedItem()=="80%")
        {
            Duty_Cycle_PWM_2=0.8;

        }
        if(DutyCycle_2.getSelectedItem()=="90%")
        {
            Duty_Cycle_PWM_2=0.9;

        }
        if(DutyCycle_2.getSelectedItem()=="100%")
        {
            Duty_Cycle_PWM_2=0.95;

        }

        System.out.println(Duty_Cycle_PWM_2);
    }
    String PWMCaluation() {
        //168=1us
        String Massg = null;
        try {


            double frequenz_pwm_1Value = Double.parseDouble(Frequenz_PWM_2.getText());
            double duty_cycle_pwm_1_Value = Duty_Cycle_PWM_2;
            double peroid_pwm_1Value = 1 / frequenz_pwm_1Value;

            int peroid_pwm_1Valuetimer = (int) (peroid_pwm_1Value * 168 *33* 10);

            /*
            Die Busfrequenz beträgt 168 MHz
            */            double duty_cycle_pwm_1_percent =  ((duty_cycle_pwm_1_Value)  * peroid_pwm_1Valuetimer);
            System.out.println(duty_cycle_pwm_1_percent);
            Massg = "1/" + String.valueOf(peroid_pwm_1Valuetimer) + "/" + String.valueOf((int)duty_cycle_pwm_1_percent);
            return Massg;

        } catch (Exception NumberFormatException) {
            JOptionPane.showMessageDialog(null, "Bitte Geben Sie die Frequenz und Duty Cycle  \n" );
        }
        return Massg;
    }
};