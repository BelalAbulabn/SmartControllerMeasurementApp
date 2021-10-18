package gui;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import static gui.Windo.*;
/*
  Diese Klasse dient zur Auswahl der Abtastfrequenz für die Messung.
  @para Samplingfrequenz  dieser Parameter wird formaler mit Timer berechnet.
 */
public class Frequenz_Selct_1_ComboBox implements ActionListener {
    @Override
    public void actionPerformed(ActionEvent e) {
        if (SamlingFrequenz.getSelectedItem()=="10kHz")
        {
            SamplingfrequenzNumber=10000;
            Samplingfrequenz =1350;
        }
        if (SamlingFrequenz.getSelectedItem()=="5kHz")
        {
            Samplingfrequenz =2700;
            SamplingfrequenzNumber=5000;
        }
        if (SamlingFrequenz.getSelectedItem()=="2kHz")
        {
            Samplingfrequenz =6750;
            SamplingfrequenzNumber=2000;
        }
        if (SamlingFrequenz.getSelectedItem()=="1kHz")
        {
            Samplingfrequenz =13500;
            SamplingfrequenzNumber=1000;
        }
        if (SamlingFrequenz.getSelectedItem()=="500Hz")
        {
            Samplingfrequenz =27000;
            SamplingfrequenzNumber=500;
        }


    }
}
