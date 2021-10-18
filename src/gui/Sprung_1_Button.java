package gui;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.PrintWriter;

import static gui.Windo.PeriodiofSprung;
import static gui.Windo.chosenPort;
import static send.Send_Data_Button.sendtoport;
/*
Diese Klasse ist verantwortlich für Senden die Sprungdauer.
 */
public class Sprung_1_Button implements ActionListener {
    @Override
    public void actionPerformed(ActionEvent e) {
        try {

            sendtoport("4/1/"+PeriodiofSprung+"\r");
        }catch (Exception NullPointerException){
            JOptionPane.showMessageDialog(null, "Bitte überprüfung Sie die Verbindung mit dem richrigen Port   \n" );
        }

    }
}
