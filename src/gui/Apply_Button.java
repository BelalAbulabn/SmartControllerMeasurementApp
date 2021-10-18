package gui;

import org.jfree.chart.ChartPanel;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.PrintWriter;


import static gui.Windo.*;
import static send.Send_Data_Button.sendtoport;
/*
Diese Klasse ist verantwortlich für das Einlesen der beiden Parameter (Abtastfrequenz und Zeitfenster)
und deren Umwandlung in Werte die der Mikrocontroller verarbeiten kann.
 */
public class Apply_Button implements ActionListener {
    @Override
    public void actionPerformed(ActionEvent e) {
        String ADCcontrol;

        SamplingFrequenz.getText();
        number_of_samples.getText();
        double calculaate_number_of_samples;

        sampledauer = PeriodinMileroSec / SamplingfrequenzNumber;
        calculaate_number_of_samples = (PeriodinMileroSec * SamplingfrequenzNumber) / (1000);
         /*
            Hier wird das X-Label entsprechend den Werten der Abtastfrequenz und des Zeitfensters aktualisiert.
           */
        XPlot();

        createChart(dataset, XPlot());
        forchart.removeAll();
        forchart.add(new ChartPanel(createChart(dataset, XPlot())), BorderLayout.CENTER);
        ;
        ADCcontrol = "3/" + (int) Samplingfrequenz + "/" + (int) calculaate_number_of_samples;
        if (calculaate_number_of_samples > 30000) {
            JOptionPane.showMessageDialog(null, "Bitte wählen Sie kleinere Parameter  die Speicher platz auf den Mikrocnrller ist begrnzt    \n");

        }


        sendtoport(ADCcontrol);

        MeasrumentStep_Windo x= new MeasrumentStep_Windo();
        x.setVisible(true);

    }
}
