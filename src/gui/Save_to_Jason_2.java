package gui;

import JASON.Profilesettings_FatherClass;
import com.github.cliftonlabs.json_simple.Jsoner;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

import static CSV.ReaderCSVtoArray_Funktions.ReadallRowCsv;
import static gui.Windo.*;


class savetojason implements ActionListener {
        savetojason() {
        }

        @Override
        public void actionPerformed(ActionEvent e) {
            String Volt = Volte_Value.getText();
            String Timeon = Time_On.getText();
            String Timein = Time_In.getText();
            String Timeout = Time_Out.getText();
            String Timepeak = Time_Peak.getText();
            String frequency = Frequency.getText();
            String Numberofpulses = Number_of_Pulses.getText();
            Profilesettings_FatherClass pr = new Profilesettings_FatherClass(Volt, Timeon, Timein, Timeout, Timepeak, frequency, Numberofpulses);
            saveDataMethod(pr.json("Firstoutput"));
            saveCSV();
        }
    public void saveDataMethod(Object o) {
        JFileChooser SaFileDialog = new JFileChooser();
        SaFileDialog.setVisible(true);

        if (SaFileDialog.showSaveDialog(null) == JFileChooser.APPROVE_OPTION) {
            File theFile = SaFileDialog.getSelectedFile();
            try {
                BufferedWriter writer = Files.newBufferedWriter(Paths.get(String.valueOf(theFile.getPath())));

                Jsoner.serialize(o, writer);

                //close the writer
                writer.close();

            } catch (Exception ex)   // any other error
            {
                JOptionPane.showMessageDialog(null, "Es gab einen Fehler!\n" + ex.getMessage());
            }
        }
    }
    void saveCSV()
    {
        JFileChooser SaFileDialog = new JFileChooser();
        SaFileDialog.setVisible(true);

        if (SaFileDialog.showSaveDialog(null) == JFileChooser.APPROVE_OPTION) {
            File theFile = SaFileDialog.getSelectedFile();
            try {
                BufferedReader writer = Files.newBufferedReader(Path.of((theFile.getPath())));
                String FileLoction;
                FileLoction=theFile.getPath();
                String tat[]= ReadallRowCsv(FileLoction);

                writer.close();

            } catch (Exception ex)   // any other error
            {
                JOptionPane.showMessageDialog(null, "Es gab einen Fehler!\n" + ex.getMessage());
            }
        }
    }
    }

