package JASON;

import com.github.cliftonlabs.json_simple.JsonObject;
import com.github.cliftonlabs.json_simple.Jsoner;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.Reader;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Map;

import static gui.Windo.*;
/*
Diese Klasse ist für die Öffnung der Profileinstellungen in einem JASON-Formular zuständig.
 */
public class Open_Jason_Button implements ActionListener {
    public Open_Jason_Button() {
    }

    @Override
    public void actionPerformed(ActionEvent e) {

        openDataMethod();

    }
    public void openDataMethod() {


        JFileChooser OpFileDialog = new JFileChooser();
        OpFileDialog.setVisible(true);

        if (OpFileDialog.showOpenDialog(null) == JFileChooser.APPROVE_OPTION) {
            File theFile = OpFileDialog.getSelectedFile();
            try {
                Reader reader = Files.newBufferedReader(Paths.get(String.valueOf(theFile.getPath())));
                JsonObject parser = (JsonObject) Jsoner.deserialize(reader);
                String name = (String) parser.get("output");
                Map<Object, Object> Output = (Map<Object, Object>) parser.get("Firstoutput");
                if (Output != null) {
                    Volte_Value.setText((String) Output.get("volte Value"));
                    Time_On.setText((String) Output.get("Time on"));
                    Time_In.setText((String) Output.get("Time in "));
                    Time_Out.setText((String) Output.get("Time out"));
                    Time_Peak.setText((String) Output.get("Time Peak"));
                    Frequency.setText((String) Output.get("Frquency"));
                    Number_of_Pulses.setText((String) Output.get("Numberofpulsess"));
                }
                if (Output == null) {
                    Map<Object, Object> Output2 = (Map<Object, Object>) parser.get("Secendoutput");
                    Volte_Value_2.setText((String) Output2.get("volte Value"));
                    Time_on_2.setText((String) Output2.get("Time on"));
                    Time_In_2.setText((String) Output2.get("Time in "));
                    Time_Out_2.setText((String) Output2.get("Time out"));
                    Time_Peak_2.setText((String) Output2.get("Time Peak"));
                    Frequency_2.setText((String) Output2.get("Frquency"));
                    Number_of_Pulses_2.setText((String) Output2.get("Numberofpulsess"));

                }


                reader.close();
            } catch (Exception ex)   // any other error (e.g. file in use, wrong file, ...)
            {

                JOptionPane.showMessageDialog(null, "Es gab einen Fehler! \n" + ex.getMessage());
            }

        }
    }
}