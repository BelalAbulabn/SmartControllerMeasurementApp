package gui;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;

import static CSV.ReaderCSVtoArray_Funktions.ReadCsv;
import static CSV.WriterCSVFromTxt_Funktions.WriteEvreyChanalInSaparateColom;
/*
In dieser Klasse werden die Mess- und Anzeigefunktionen ausgeführt.
 */
public class StartMeasumernt_Button implements ActionListener {
    @Override
    public void actionPerformed(ActionEvent e) {




        try {
            WriteEvreyChanalInSaparateColom();
        } catch (IOException ioException) {
            ioException.printStackTrace();
        }

        ReadCsv();

    }
}
