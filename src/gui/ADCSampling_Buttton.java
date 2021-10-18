package gui;

import org.jfree.chart.ChartPanel;
import static gui.Windo.*;
import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class ADCSampling_Buttton implements ActionListener {
    @Override
    public void actionPerformed(ActionEvent e) {

            String ADCcontrol;


            double calculaate_Samplingfrequenz;


            double calculaate_number_of_samples;
            calculaate_number_of_samples=(PeriodinMileroSec*SamplingfrequenzNumber)/(1000);



            XPlot();

            createChart(dataset,XPlot());
            forchart.removeAll();
            forchart.add(new ChartPanel(createChart(dataset,XPlot())), BorderLayout.CENTER);


            ADCcontrol="3/"+(int)Samplingfrequenz+"/"+(int)calculaate_number_of_samples;
            if (calculaate_number_of_samples>30000)
            {
                JOptionPane.showMessageDialog(null, "Bitte wählen Sie kleinere Parameter  die Speicher platz auf den Mikrocnrller ist begrnzt    \n" );

            }

            analog=ADCcontrol;
        }

    }

