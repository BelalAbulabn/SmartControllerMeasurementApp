package gui;

import Chart.plotTheThirdChannle_Button;
import Chart.plotTheSecondChannle_Button;
import Chart.plotTheFirstChannle_Button;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class MeasrumentStep_Windo extends JDialog implements ActionListener {
    private boolean ClearCSVtext = false;
    private boolean statusFTB = false;
    private boolean statusSTB = false;
    private JTextField inputField;
    private JButton StartButton = new JButton("Start Measrument");
    private JButton plotTheFirstChannle = new JButton("plot the First Channle");
    private JButton plotTheSecondtChannle = new JButton("plot the Second Channle");
    private JButton plotTheThirdtChannle = new JButton("plot the Third Channle");

    private JLabel Samplingfrequenz=new JLabel("              ");
    //private JLabel Anweisung =new JLabel("lösche zuerst alle alte Daten     ");

    private JLabel space=new JLabel("                 ");
    private JLabel period=new JLabel("                     ");
    public MeasrumentStep_Windo() {
        super((JFrame)null,"New Measurment",true);
        setSize(200,250);
        setLocationRelativeTo(null);
        setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
        setResizable(false);
        StartButton.addActionListener(this);
        plotTheFirstChannle.addActionListener(this);
        inputField = new JTextField(20);
        JPanel contentPane = (JPanel)getContentPane();
        contentPane.setLayout(new FlowLayout());

        contentPane.add(plotTheFirstChannle);
        contentPane.add(plotTheSecondtChannle);
        contentPane.add(plotTheThirdtChannle);

        contentPane.add(space);
        Samplingfrequenz.setSize(200,30);
        contentPane.add(Samplingfrequenz);

        period.setSize(200,30);
        contentPane.add(period);

        contentPane.add(StartButton);
        StartButton.addActionListener(new StartMeasumernt_Button());
        plotTheFirstChannle.addActionListener(new plotTheFirstChannle_Button());
        plotTheSecondtChannle.addActionListener(new plotTheSecondChannle_Button());
        plotTheThirdtChannle.addActionListener(new plotTheThirdChannle_Button());





    }





    public void actionPerformed(ActionEvent e){


        if (e.getActionCommand().equals("Start Measrument")) {
            if (ClearCSVtext == true) {
               // okButton.addActionListener(new StartMeasumernt());

                ClearCSVtext = true;
                dispose();

            }


        }




    }
/*
    Exception NullPointerException) {
        JOptionPane.showMessageDialog(null, "Überprufen Sie die Verbinung mit den rechtigen Port  \n");

 */
}