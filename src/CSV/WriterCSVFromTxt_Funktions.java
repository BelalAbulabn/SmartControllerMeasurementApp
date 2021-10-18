package CSV;


import java.io.*;
import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import static gui.Windo.filePath;
import static gui.Windo.filePathtex;


      /*
        In der Datei data.txt wird alles, was vom Mikrocontroller an den Host-PC gesendet wird, zwischengespeichert.
          Diese Funktion dient dazu, die data.txt zu lesen und daraus die Werte der Analogeingänge auszuschneiden und zu unterscheiden.
          Dies geschieht in folgenden Schritten.
           1. Zählen Sie die Zeilen aus Data.txt.
           2. Erstellen Sie daraus eine Array-Liste.
           3. Suche nach einem bestimmten Muster, das jeden Kanal in der
           Array-Liste unterscheiden kann.
           4.schneide die Werte entsprechend dem Muster aus
           5.speichere die ausgeschnittenen Werte in dem Array
           6.jeder Kanal hat ein Array.
           7.jede Array wird in einer Spalte geschrieben
           8.jedes Array wird in eine Spalte der CSV-Datei geschrieben
          */


    public class WriterCSVFromTxt_Funktions {

        static int maxlength;
        static File file = new File(filePathtex);

        static String csvFile =filePath;
        private static ArrayList data  = (ArrayList)FindeNumberOfRow().clone();
        private static int NumberOfRow =data.size();
        private static ArrayList  FindeNumberOfRow() {


            ArrayList Data = new ArrayList<String>();
            int x = 0;
            Scanner scan = null;
            try {
                scan = new Scanner(file);
            } catch (FileNotFoundException fileNotFoundException) {
                fileNotFoundException.printStackTrace();
            }
            while (scan.hasNextLine()) {
                String line = scan.nextLine();
                String str[] = line.split(",");
                List<String> al = new ArrayList<String>();
                al = Arrays.asList(str);
                for (String s : al) {
                    Data.add(x, s);
                    x++;
                    try {
                    } catch (NumberFormatException ignore) {
                    }
                }
            }
            int numberOfRow = Data.size();


            return Data;
        }



        public static String[] CreataArrayForFirstChanal() {
            String Firstchanal[] = new String[NumberOfRow];
            Firstchanal[0] = "Analog_IN_1";
            String a = null;
            Pattern p = Pattern.compile("(?<=PA0->Analog_IN_1)\\d*.\\d*");
            List<String> ne = new ArrayList<String>();
            FindeNumberOfRow().clear();
            ne = FindeNumberOfRow();
            List<String[]> list = new ArrayList<>();
            List<String> ac = new ArrayList<String>();
            int x = 1;
            try {
                for (String s : ne) {
                    Matcher m = p.matcher(s);

                    if ((m.find())) {
                        a = m.group();
                        Firstchanal[x] = a;
                    }
                    x++;
                }
            } catch (Exception ArrayIndexOutOfBoundsException) {

            }
            Firstchanal = Arrays.stream(Firstchanal)
                    .filter(s -> (s != null && s.length() > 0))
                    .toArray(String[]::new);

            return Firstchanal;
        }




        public static String Notifymassege() {
            String Firstchanal[] = new String[NumberOfRow];
            Firstchanal[0] = "Analog_IN_1";
            String a = null;
            Pattern p = Pattern.compile("(?<=Sending is done)");
            List<String> ne = new ArrayList<String>();
            ne = data;
            List<String[]> list = new ArrayList<>();
            List<String> ac = new ArrayList<String>();
            int x = 1;
            try {
                for (String s : ne) {
                    Matcher m = p.matcher(s);

                    if ((m.find())) {
                        a= " Alle Messdaten sind da ,jetzt kannst auf Plot drücken";
                    }
                    
                }
            } catch (Exception ArrayIndexOutOfBoundsException) {

            }
            
            return a;
        }
        public static String[] CreataArrayForSecendChanal() {
            String SecendChanal[] = new String[NumberOfRow];
            SecendChanal[0] = "Analog_IN_2";
            String a = null;
            Pattern p = Pattern.compile("(?<=PA1->Analog_IN_2)\\d*.\\d*");
            List<String> ne = new ArrayList<String>();
            ne = data;
            List<String[]> list = new ArrayList<>();
            List<String> ac = new ArrayList<String>();
            int x = 1;
            try {
                for (String s : ne) {
                    Matcher m = p.matcher(s);

                    if ((m.find())) {
                        a = m.group();
                        SecendChanal[x] = a;
                    }
                    x++;
                }
            } catch (Exception ArrayIndexOutOfBoundsException) {

            }
            SecendChanal = Arrays.stream(SecendChanal)
                    .filter(s -> (s != null && s.length() > 0))
                    .toArray(String[]::new);

            return SecendChanal;
        }

        public static String[] CreataArrayForThirdChanal() {
            String ThirdChanal[] = new String[NumberOfRow];
            ThirdChanal[0] = "Analog_IN_3";
            String a = null;
            Pattern p = Pattern.compile("(?<=PA2->Analog_IN_3)\\d*.\\d*");
            List<String> ne = new ArrayList<String>();
            ne = data;
            List<String[]> list = new ArrayList<>();
            List<String> ac = new ArrayList<String>();
            int x = 1;
            try {
                for (String s : ne) {
                    Matcher m = p.matcher(s);

                    if ((m.find())) {
                        a = m.group();
                        ThirdChanal[x] = a;
                    }
                    x++;
                }
            } catch (Exception ArrayIndexOutOfBoundsException) {

            }
            ThirdChanal = Arrays.stream(ThirdChanal)
                    .filter(s -> (s != null && s.length() > 0))
                    .toArray(String[]::new);

            return ThirdChanal;
        }
        static public int FindeTheShortstLength(){
            int TheShortstLength;
            Integer[] FindeTheShortstLength = {CreataArrayForFirstChanal().length, CreataArrayForSecendChanal().length, CreataArrayForThirdChanal().length};
            int minlength = Collections.min(Arrays.asList(FindeTheShortstLength));
            TheShortstLength=minlength;
            return TheShortstLength;
        }
        static public int FindeThelongestLength(){
            int TheLongestLength;
            Integer[] FindeTheShortstLength = {CreataArrayForFirstChanal().length, CreataArrayForSecendChanal().length, CreataArrayForThirdChanal().length};
            int maxlength = Collections.max(Arrays.asList(FindeTheShortstLength));
            TheLongestLength=maxlength;
            return TheLongestLength;
        }



      static  public void WriteEvreyChanalInSaparateColom() throws IOException {
          Arrays.fill(CreataArrayForFirstChanal(),null);
          Arrays.fill(CreataArrayForSecendChanal(),null);
          Arrays.fill(CreataArrayForThirdChanal(),null);

          CreataArrayForFirstChanal();
          CreataArrayForSecendChanal();
          CreataArrayForThirdChanal();
            String FirstChanal[]=Arrays.copyOf(CreataArrayForFirstChanal(),CreataArrayForFirstChanal().length);
            String SecndChanal[]=Arrays.copyOf(CreataArrayForSecendChanal(),CreataArrayForSecendChanal().length);
            String ThridChanal[]=Arrays.copyOf(CreataArrayForThirdChanal(),CreataArrayForThirdChanal().length);
            int TheMinArray= FindeTheShortstLength();

              BufferedWriter br = null;
            try {
                br = new BufferedWriter(new FileWriter(csvFile));
            } catch (IOException e) {
                e.printStackTrace();
            }
            StringBuilder sb = new StringBuilder();



                try {
                    for (int i = 0; i < TheMinArray; i++) {

                        sb.append(FirstChanal[i] + ';');
                        sb.append(SecndChanal[i] + ";");
                        sb.append(ThridChanal[i] + ";");
                        sb.append("\n");

                    }
                } catch (Exception ArrayIndexOutOfBoundsException) {
                    ArrayIndexOutOfBoundsException.printStackTrace();
                }

                br.write(sb.toString());
                br.close();
              //Um ArrayIndexOutOfBoundsException zu wrmeinden wwid nach kleinste Array gescht
            }
        static  public void WriteEvreyChanalInSaparateColomtoCSV(String FirstChanal[],String SecndChanal[],String ThridChanal[],String Path) throws IOException {

            int TheMinArray= FindeTheShortstLength();

            BufferedWriter br = null;
            try {
                br = new BufferedWriter(new FileWriter(Path));
            } catch (IOException e) {
                e.printStackTrace();
            }
            StringBuilder sb = new StringBuilder();



            try {
                for (int i = 0; i < TheMinArray; i++) {

                    sb.append(FirstChanal[i] + ';');
                    sb.append(SecndChanal[i] + ";");
                    sb.append(ThridChanal[i] + ";");
                    sb.append("\n");

                }
            } catch (Exception ArrayIndexOutOfBoundsException) {
                ArrayIndexOutOfBoundsException.printStackTrace();
            }

            br.write(sb.toString());
            br.close();
            //Um ArrayIndexOutOfBoundsException zu wrmeinden wwid nach kleinste Array gescht
        }


        public static void data() throws IOException {


            String wer[] = new String[NumberOfRow];
            String wer1[] = new String[NumberOfRow];
            String wer2[] = new String[NumberOfRow];
            wer[0] = "Analog_IN_1";
            wer1[0] = "Analog_IN_2";
            wer2[0] = "Analog_IN_3";

            String a = null;
            String b = null;
            String c = null;

            Pattern p = Pattern.compile("(?<=PA0->Analog_IN_1)\\d*.\\d*");
            Pattern p1 = Pattern.compile("(?<=PA1->Analog_IN_2)\\d*.\\d*");
            Pattern p2 = Pattern.compile("(?<=PA2->Analog_IN_3)\\d*.\\d*");
            List<String> ne = new ArrayList<String>();
            ne = data;
            List<String[]> list = new ArrayList<>();
            List<String> ac = new ArrayList<String>();
            int x = 1;
            try {
                for (String s : ne) {
                    Matcher m = p.matcher(s);
                    Matcher l = p1.matcher(s);
                    Matcher o = p2.matcher(s);
                    if ((m.find())) {
                        a = m.group();
                        wer[x] = a;
                    }
                    if ((l.find())) {
                        b = l.group();
                        wer1[x] = b;
                    }
                    if ((o.find())) {
                        c = o.group();
                        wer2[x] = c;
                    }

                    x++;
                }
            } catch (Exception ArrayIndexOutOfBoundsException) {

            }

            wer = Arrays.stream(wer)
                    .filter(s -> (s != null && s.length() > 0))
                    .toArray(String[]::new);
            wer1 = Arrays.stream(wer1)
                    .filter(s -> (s != null && s.length() > 0))
                    .toArray(String[]::new);
            wer2 = Arrays.stream(wer2)
                    .filter(s -> (s != null && s.length() > 0))
                    .toArray(String[]::new);

            BufferedWriter br = new BufferedWriter(new FileWriter(csvFile));

            StringBuilder sb = new StringBuilder();


            Integer[] Findthecorrectlengt = {wer1.length, wer2.length, wer1.length};
            int minlength = Collections.min(Arrays.asList(Findthecorrectlengt));
            maxlength = Collections.max(Arrays.asList(Findthecorrectlengt));
            try {
                for (int i = 0; i < minlength; i++) {

                    sb.append(wer[i] + ';');
                    sb.append(wer1[i] + ";");
                    sb.append(wer2[i] + ";");
                    sb.append("\n");

                }
            } catch (Exception ArrayIndexOutOfBoundsException) {
                ArrayIndexOutOfBoundsException.printStackTrace();
            }
            br.write(sb.toString());
            br.close();

            System.out.println("Array mit ADC Werte Fertig");
        }


    }