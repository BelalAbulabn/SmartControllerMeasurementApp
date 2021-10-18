package CSV;

import java.io.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import static gui.Windo.filePath;
/*#############################################################################*/
/**
 *
 * 1.Liest alle Daten aus CSV-Daten
 * 2.teilt die Spalten in ein Array ein
 * 3.bereitet das Array zum Lesen und Plotten vor
 *
 */
/*#############################################################################*/
public class ReaderCSVtoArray_Funktions {
    private static final String SAMPLE_CSV_FILE_PATH = filePath;






    public static int Datanumber;
    List<String[]> dataLines = new ArrayList<>();
    static int   NumberOfRowCsv=  FindeNumberOfRowCsv();
    public static  String [] PlotForFirstChart =new String[NumberOfRowCsv];
    public static  String [] PlotForSecndChart =new  String[NumberOfRowCsv];
    public static  String [] PlotForThirdChart =new  String[NumberOfRowCsv];

    public static int FindeNumberOfRowCsv(){
        int NumberOfRowCsv;
        int x=0;
        String Lines;
        BufferedReader br = null;
        try {
            br = new BufferedReader(new FileReader(SAMPLE_CSV_FILE_PATH));
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
        while (true) {
            try {
                if (!((Lines = br.readLine()) != null)) break;
            } catch (IOException e) {
                e.printStackTrace();
            }
            x++;
        }
        NumberOfRowCsv=x;
        return NumberOfRowCsv;

    }

    public static void ReadCsv() {
        int x=0;
        String Lines = null;
               BufferedReader br = null;
        try {
            br = new BufferedReader(new FileReader(SAMPLE_CSV_FILE_PATH));
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }

        while (true) {
            // liese die ganze CSV Zeile nach die Andere
            try {
                if (!((Lines = br.readLine()) != null)) break;
            } catch (IOException e) {
                e.printStackTrace();
            }
            // use comma as separator

            String[] cols = Lines.split(";");

/**
 * jede Spalte in Array teilen
 */

            PlotForFirstChart[x]=cols[0];
            PlotForSecndChart[x]=cols[1];
            PlotForThirdChart[x]=cols[2];
            x++;
        }


    }
    public static String[] ReadallRowCsv(String Path){

        String[] NumberOfRowCsv1 = new String[1000000];
        int x=0;
        String Lines;
        BufferedReader br = null;
        try {
            br = new BufferedReader(new FileReader(Path));
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
        while (true) {
            try {
                if (!((Lines = br.readLine()) != null)) break;

                NumberOfRowCsv1[x]=Lines;
                x++;
            } catch (IOException e) {
                e.printStackTrace();
            }

        }

        return NumberOfRowCsv1;

    }






}


