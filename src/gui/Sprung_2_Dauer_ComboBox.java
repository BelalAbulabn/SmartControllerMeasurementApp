package gui;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.time.Period;

import static gui.Windo.*;
/*
    Diese Klasse ist verantwortlich für die Auswahl der Sprungdauer .
    Für die zweite  Vollbrückenschaltung
 */
public class Sprung_2_Dauer_ComboBox implements ActionListener {
    @Override
    public void actionPerformed(ActionEvent e) {
        if (Sprung_Dauer_2.getSelectedItem() == "100ms") {
            PeriodiofSprung = 1;
        }
        if (Sprung_Dauer_2.getSelectedItem() == "200ms") {
            PeriodiofSprung = 2;
        }
        if (Sprung_Dauer_2.getSelectedItem() == "300ms") {
            PeriodiofSprung = 3;
        }
        if (Sprung_Dauer_2.getSelectedItem() == "400ms") {
            PeriodiofSprung = 4;
        }
        if (Sprung_Dauer_2.getSelectedItem() == "500ms") {
            PeriodiofSprung = 5;
        }
        if (Sprung_Dauer_2.getSelectedItem() == "1s") {
            PeriodiofSprung = 10;
        }
        if (Sprung_Dauer_2.getSelectedItem() == "2s") {
            PeriodiofSprung = 20;
        }
        if (Sprung_Dauer_2.getSelectedItem() == "3s") {
            PeriodiofSprung = 30;

        }

    }
}