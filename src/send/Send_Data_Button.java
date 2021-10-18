package send;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Scanner;

import static gui.Windo.*;
/*
Die Funktion sendtoport ist dafür zuständig,
die Befehle an den Mikrocontroller zu senden und gleichzeitig die Antwort zu empfangen.
 */

public class Send_Data_Button implements ActionListener {
    public static Scanner scanner;

    public Send_Data_Button() {
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        sendtoport(analog);

    }
    public static void sendtoport(String ADC) {


        Thread thread = new Thread() {
            public void run() {


                try {
                    PrintWriter output = new PrintWriter(chosenPort.getOutputStream());
                    output.print(ADC + "\r");
                    System.out.println(ADC);
                    output.flush();
                    output.close();


                } catch (
                        Exception NullPointerException) {
                    JOptionPane.showMessageDialog(null, "Überprufen Sie die Verbinung mit den rechtigen Port  \n");

                }


                try {
                    if (recevdatat() == true) ;
                } catch (
                        FileNotFoundException fileNotFoundException) {
                    fileNotFoundException.printStackTrace();
                }
                try {
                    delete();
                } catch (
                        Exception exception) {
                    exception.printStackTrace();
                }
            }

        };
        thread.start();


    }
    static boolean recevdatat() throws FileNotFoundException {

        //Thread thread1 = new Thread() {
        //public void run() {

        scanner = new Scanner(chosenPort.getInputStream());
        //JOptionPane.showMessageDialog(null, "Bitte Warten Sie Kurz bis alle Daten gesendet ist   \n" );
        while (scanner.hasNextLine()) {
            try {


                String line = scanner.nextLine();

                FileOutputStream fx = new FileOutputStream("data.txt", true);
                PrintWriter pw = new PrintWriter(fx);
                //System.out.println(line)


                String str[] = line.split(",");
                List<String> al = new ArrayList<String>();
                //	System.out.println(al);
                al = Arrays.asList(str);

                for (String s : al) {
                    System.out.println(s);
                    pw.println(s);
                }
                pw.close();
            } catch (Exception e) {
            }
        }
        scanner.close();
        return true;
    }
    static void delete() throws Exception {
        String result = fileToString(filePath);

        //Replacing the word with desired one
        result = result.replace(result, "");
        //Rewriting the contents of the file
        PrintWriter writer = new PrintWriter(new File(filePath));
        writer.append(result);
        writer.flush();
    }
}