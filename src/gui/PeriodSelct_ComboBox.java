package gui;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.time.Period;

import static gui.Windo.*;
/*

Diese Klasse ist für die Auswahl der Zeitfenster der Messung
@ PeriodinMileroSec wird in milseckonde gerchnet
 */
public class PeriodSelct_ComboBox implements ActionListener {
    @Override
    public void actionPerformed(ActionEvent e) {

            if (Period.getSelectedItem() == "100ms") {
                PeriodinMileroSec=100;
            }
            if (Period.getSelectedItem() == "200ms") {
                PeriodinMileroSec=200;
            }
            if (Period.getSelectedItem() == "500ms") {
                PeriodinMileroSec=500;
            }
            if (Period.getSelectedItem() == "1s") {
                PeriodinMileroSec=1000;
            }
            if (Period.getSelectedItem() == "2s") {
                PeriodinMileroSec=2000;
            }
            if (Period.getSelectedItem() == "3s") {
                PeriodinMileroSec=3000;
            }



    }
}
