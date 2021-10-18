package gui;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.PrintWriter;

import static gui.Windo.PeriodiofSprung;
import static gui.Windo.chosenPort;
import static send.Send_Data_Button.sendtoport;
/*#############################################################################*/
/**
 send zu Sprung funktion
 */
/*#############################################################################*/
public class Sprung_2_Button implements ActionListener {
    @Override
    public void actionPerformed(ActionEvent e) {
        sendtoport("4/2/"+PeriodiofSprung);

        System.out.println("4/2/"+PeriodiofSprung+"\r");

    }
}
