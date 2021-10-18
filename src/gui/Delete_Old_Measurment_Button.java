package gui;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;

import static gui.Windo.*;
/*
Diese Klasse ist für die Löschung der alten Daten zuständig.
 */
public class Delete_Old_Measurment_Button implements ActionListener {
    @Override
    public void actionPerformed(ActionEvent e) {



        String result = null;
            try {
                result = fileToString(filePath);
            } catch (Exception exception) {
                exception.printStackTrace();
            }

            //Replacing the word with desired one
            result = result.replace(result, "");
            //Rewriting the contents of the file
            PrintWriter writer = null;
            try {
                writer = new PrintWriter(new File(filePathtex));
                writer = new PrintWriter(new File(filePath));
            } catch (FileNotFoundException fileNotFoundException) {
                fileNotFoundException.printStackTrace();
            }
            writer.append(result);
            writer.flush();
        }


}
