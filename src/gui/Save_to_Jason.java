package gui;

import JASON.Profilesettings_FatherClass;
import com.github.cliftonlabs.json_simple.Jsoner;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedWriter;
import java.io.File;
import java.nio.file.Files;
import java.nio.file.Paths;

import static gui.Windo.*;

class savetojason2 implements ActionListener {
    public savetojason2() {
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        String Volt = Volte_Value.getText();


        String Volt2 = Volte_Value_2.getText();
        String Timeon2 = Time_on_2.getText();
        String Timein2 = Time_In_2.getText();
        String Timeout2 = Time_Out_2.getText();
        String Timepeak2 = Time_Peak_2.getText();
        String frequency2 = Frequency_2.getText();
        String Numberofpulses2 = Number_of_Pulses_2.getText();
        Profilesettings_FatherClass pr2 = new Profilesettings_FatherClass(Volt2, Timeon2, Timein2, Timeout2, Timepeak2, frequency2, Numberofpulses2);

        saveDataMethod(pr2.json("Secendoutput"));

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
}