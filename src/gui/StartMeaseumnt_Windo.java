package gui;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import static gui.Windo.*;
/*
Das erste Fenster für den Beginn der Messungen
 */
public class StartMeaseumnt_Windo extends JDialog implements ActionListener {
    private boolean ClearCSVtext = false;

    private JTextField inputField;
    private JButton Apply_Button = new JButton("Apply");
    private JButton ClearButton = new JButton("Clear");
    private JRadioButton FitnessTestButton = new JRadioButton("Fitness Test");
    private JRadioButton SchellongTestButton = new JRadioButton("Schellong Test");

    private JLabel Samplingfrequenz=new JLabel("Samplingfrequenz");
    private JLabel Anweisung =new JLabel("lösche zuerst alle alte Daten     ");

    private JLabel space=new JLabel("                 ");
    private JLabel period=new JLabel("period                   ");
    public StartMeaseumnt_Windo() {
        super((JFrame)null,"New Measurment",true);
        setSize(200,200);
        setLocationRelativeTo(null);
        setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
        setResizable(false);

        ClearButton.addActionListener(this);
        FitnessTestButton.addActionListener(this);

        SchellongTestButton.addActionListener(this);
        inputField = new JTextField(20);
        JPanel contentPane = (JPanel)getContentPane();
        contentPane.setLayout(new FlowLayout());
        contentPane.add(Anweisung);
        contentPane.add(ClearButton);
        contentPane.add(space);
        Samplingfrequenz.setSize(200,30);
        contentPane.add(Samplingfrequenz);
        SamlingFrequenz.setSize(100,30);
        contentPane.add(SamlingFrequenz);
        period.setSize(200,30);
        contentPane.add(period);
        Period.setSize(100,30);
        contentPane.add(Period);
        contentPane.add(Apply_Button);
        ClearButton.addActionListener(new Delete_Old_Measurment_Button());

        Apply_Button.addActionListener(new Apply_Button());
      //  Apply_Button.addActionListener(this);
    }





    public void actionPerformed(ActionEvent e){


    if (e.getActionCommand().equals("Apply")) {
        if (ClearCSVtext == true) {
            //okButton.addActionListener(new Apply_Button());
            if (chosenPort == null) {
                JOptionPane.showMessageDialog(null, "Überprufen Sie die Verbinung mit den rechtigen Port  \n");

            }


            // if(  (inputField.getText().isEmpty() == false) && ((statusFTB == true) || (statusSTB == true))  ) {
            ClearCSVtext = true;
            dispose();

            //}

        }

        if (ClearCSVtext == false) {
            JOptionPane.showMessageDialog(null, "zuerst müssen die alten Daten gelöscht werden Klick auf Clear  \n");

        }
    }
        if(e.getActionCommand().equals("Clear")){

            ClearCSVtext = true;

        }





    }

}