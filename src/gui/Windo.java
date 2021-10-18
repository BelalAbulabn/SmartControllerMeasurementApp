package gui;
/*
ALLE BENÖTIGTEN BIBLIOTHEKEN
 */
import java.awt.*;

import javax.swing.*;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import javax.swing.ImageIcon;

import CSV.ReaderCSVtoArray_Funktions;
import Chart.plotTheFirstChannle_Button;
import JASON.Open_Jason_Button;
import communication.Refresh_Button;
import communication.Openport_Button;
import org.jfree.chart.ChartFactory;
import com.fazecast.jSerialComm.SerialPort;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.NumberAxis;
import org.jfree.chart.axis.NumberTickUnit;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.chart.plot.XYPlot;
import org.jfree.data.xy.XYDataset;
import org.jfree.data.xy.XYSeries;
import org.jfree.data.xy.XYSeriesCollection;
import send.Send_Data_Button;
import java.nio.file.Files;
import java.nio.file.Path;
import java.io.*;
import java.util.*;
import java.util.List;

import static CSV.ReaderCSVtoArray_Funktions.*;


public class Windo implements Observer {

	/*
Beim ersten Kompilieren müssen Sie eine txt-Datei und eine CSV-Datendatei erstellen.
Die Datei muss erstellt und ihr Pfad hier eingetragen werden.
 */
	public static String filePath = "C:\\Users\\bilal\\Dropbox\\Abscluss\\Smartcontrollerr\\users.csv";
	public static String filePathtex = "C:\\Users\\bilal\\Dropbox\\Abscluss\\Smartcontrollerr\\data.txt";
/*
Komponenten der GUI
 */
	private JFrame frame;
	private JFrame frame1;
	static public JTextField Volte_Value;
	static public JTextField Time_On;
	static public JTextField Time_In;
	static public JTextField Time_Out;
	static public JTextField Time_Peak;
	static public JTextField Frequency;
	static public JTextField Number_of_Pulses;
	static public JTextField Volte_Value_2;
	static public JTextField Time_on_2;
	static public JTextField Time_In_2;
	static public JTextField Time_Out_2;
	static public JTextField Time_Peak_2;
	static public JTextField Frequency_2;
	static public JTextField Number_of_Pulses_2;
	static public JTextField Frequenz_PWM_1;
	static public JTextField Frequenz_PWM_2;
	ImageIcon imagePWM = new ImageIcon("PWM.PNG");
	ImageIcon imageTrapez = new ImageIcon("Trapez.PNG");
	public static JComboBox<String> Portlist = new JComboBox();
	public static JButton ConnectButton = new JButton("Connect");
	JButton UpdateButton = new JButton("Update");
	JButton PlotButton = new JButton("Plot");
	JButton sendButton = new JButton("send");
	JButton saveButton = new JButton("Save");
	JButton OpenJson1 = new JButton("OpenJson");
	JButton SaveJason1 = new JButton("SaveJason");
	JButton OpenJson2 = new JButton("OpenJson");
	JButton SaveJason2 = new JButton("SaveJason");
	static JButton Start_PWM_1_Full_bridge = new JButton("Start(voll)");
	static JButton Start_PWM_1_half_bridge = new JButton("Start(halb)");
	static JButton Start_PWM_2_half_bridge = new JButton("Start(halb)");
	static JButton Start_PWM_2_Full_bridge = new JButton("Start(voll)");

	JTextPane empfangen = new JTextPane();
	static String[] DutyCycleSelct ={"5%","10%","20%","30%","40%","50%","60%","70%","80%","90%","100%"};
	static String [] FrequenzSelct={"10kHz","5kHz","2kHz","1kHz","500Hz"};
	static JComboBox SamlingFrequenz = new JComboBox(FrequenzSelct);
	static public JComboBox DutyCycle = new JComboBox(DutyCycleSelct);
	static public JComboBox DutyCycle_2 = new JComboBox(DutyCycleSelct);
	SerialPort[] portNames = SerialPort.getCommPorts();
	public static Scanner scanner;
	public static SerialPort chosenPort;
	Enumeration enumComm;
	public static String analog;
	static JTabbedPane forchart = new JTabbedPane(JTabbedPane.TOP);
	public static XYSeries series1 = new XYSeries("Analog Eingan(1)");
	public static XYSeries series2 = new XYSeries("Analog Eingan(2)");
	public static XYSeries series3 = new XYSeries("Analog Eingan(3)");
	String xAxisLabelunit ="Zeit (mS)";
	public static XYSeriesCollection dataset = new XYSeriesCollection();
	static int Samplingfrequenz = 1350;
	static double SamplingfrequenzNumber= 10000;
	static double PeriodinMileroSec=1000;
	static int PeriodiofSprung=0;
	static double sampledauer=PeriodinMileroSec/SamplingfrequenzNumber;
	static public double Duty_Cycle_PWM_1;
	static public double Duty_Cycle_PWM_2;
	static String []Periodselct={"100ms","200ms","500ms","1s","2s","3s"};
	static String []PeriodselctSprung={"100ms","200ms","300ms","400ms","500ms","1s","2s","3s"};
	static JComboBox Sprung_Dauer_2 = new JComboBox(PeriodselctSprung);
	//static JComboBox Sprung_Dauer_1 = new JComboBox(PeriodselctSprung);
	static JComboBox Sprung_Dauer_1 = new JComboBox(PeriodselctSprung);
	static JComboBox Period = new JComboBox(Periodselct);
	static int x = 0;
	private JLabel Foto;
	public static JTextField SamplingFrequenz;
	public static JTextField number_of_samples;
	private JTextField Sprung_Number_1;
	private JTextField Sprung_Number_2;

	//	CommunicationView ss =new CommunicationView();
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					Windo window = new Windo();
					window.frame.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}

	/**
	 * Create the application.
	 */
	public Windo() {
		initialize();

	}

	/**
	 * Initialize the contents of the frame.
	 */

	private void initialize() {
		frame = new JFrame();
		frame.setSize(1700, 1021);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		GridBagLayout gridBagLayout = new GridBagLayout();
		gridBagLayout.columnWidths = new int[]{392, 0};
		gridBagLayout.rowHeights = new int[]{22, 0, 45, 15, 0, 0, 0};
		gridBagLayout.columnWeights = new double[]{1.0, Double.MIN_VALUE};
		gridBagLayout.rowWeights = new double[]{0.0, 1.0, 1.0, 1.0, 1.0, 1.0, Double.MIN_VALUE};
		frame.getContentPane().setLayout(gridBagLayout);


		JMenuBar menuBar = new JMenuBar();
		GridBagConstraints gbc_menuBar = new GridBagConstraints();
		gbc_menuBar.anchor = GridBagConstraints.NORTHWEST;
		gbc_menuBar.insets = new Insets(0, 0, 5, 0);
		gbc_menuBar.gridx = 0;
		gbc_menuBar.gridy = 0;
		frame.getContentPane().add(menuBar, gbc_menuBar);
		JMenu TheMenu = new JMenu("Menu");
		menuBar.add(TheMenu);

		JMenuItem New_measurement = new JMenuItem("New measurement");
		TheMenu.add(New_measurement);

		JMenuItem Start_measrument = new JMenuItem("Start Measrument");
		TheMenu.add(Start_measrument);

		JMenuItem Import = new JMenuItem("Import");
		TheMenu.add(Import);

		JMenuItem Save_Data = new JMenuItem("Save");
		Save_Data.setSelected(true);
		TheMenu.add(Save_Data);

		JMenuItem Open = new JMenuItem("Open");
		TheMenu.add(Open);

		JMenu menu = new JMenu("New menu");
		TheMenu.add(menu);

		JMenu mnNewMenu = new JMenu("Daten_Tools");
		menuBar.add(mnNewMenu);

		JMenuItem mntmNewMenuItem = new JMenuItem("New menu item");
		mnNewMenu.add(mntmNewMenuItem);

		JMenu mnNewMenu_1 = new JMenu("Hilfe");
		menuBar.add(mnNewMenu_1);

		JMenuItem mntmNewMenuItem_1 = new JMenuItem("New menu item");
		mnNewMenu_1.add(mntmNewMenuItem_1);

		JLayeredPane layeredPane_3 = new JLayeredPane();
		layeredPane_3.setBackground(UIManager.getColor("CheckBox.foreground"));
		GridBagConstraints gbc_layeredPane_3 = new GridBagConstraints();
		gbc_layeredPane_3.insets = new Insets(0, 0, 5, 0);
		gbc_layeredPane_3.fill = GridBagConstraints.BOTH;
		gbc_layeredPane_3.gridx = 0;
		gbc_layeredPane_3.gridy = 1;
		frame.getContentPane().add(layeredPane_3, gbc_layeredPane_3);


		JPanel panel = new JPanel();
		panel.setBounds(0, 0, 308, 129);
		layeredPane_3.add(panel);
		panel.setLayout(null);
		/*
		So suchen Sie nach allen verfügbaren COM-Ports am Host-PC
 		*/
		Portlist.setBounds(53, 46, 191, 24);
		panel.add(Portlist);
		for (int i = 0; i < Protzahl(); i++) {
			Portlist.addItem(portNames[i].getSystemPortName());

		}


		this.ConnectButton.addActionListener(new Openport_Button());
		//	window.add(new ChartPanel(chart), BorderLayout.CENTER);
		ConnectButton.setBounds(10, 11, 124, 24);
		panel.add(ConnectButton);


		saveButton.setBounds(10, 81, 108, 24);
		panel.add(saveButton);
		//this.saveButton.addActionListener(new Windo.savetojason());
		this.saveButton.addActionListener(new savetojason());
		this.UpdateButton.addActionListener(new Refresh_Button());

		UpdateButton.setBounds(168, 11, 130, 24);
		panel.add(UpdateButton);

		sendButton.setBounds(168, 81, 111, 24);
		panel.add(sendButton);


		JPanel panel_3 = new JPanel();
		panel_3.setBounds(314, 13, 431, 116);
		layeredPane_3.add(panel_3);
		panel_3.setLayout(null);

		empfangen.setBounds(10, 11, 411, 94);
		panel_3.add(empfangen);
		this.sendButton.addActionListener(new Send_Data_Button());
		JTabbedPane tabbedPane = new JTabbedPane(JTabbedPane.TOP);
		GridBagConstraints gbc_tabbedPane = new GridBagConstraints();
		gbc_tabbedPane.insets = new Insets(0, 0, 5, 0);
		gbc_tabbedPane.gridheight = 4;
		gbc_tabbedPane.fill = GridBagConstraints.BOTH;
		gbc_tabbedPane.gridx = 0;
		gbc_tabbedPane.gridy = 2;

		frame.getContentPane().add(tabbedPane, gbc_tabbedPane);

		JLayeredPane chart1 = new JLayeredPane();
		tabbedPane.addTab("Measurment", null, chart1, null);
		this.PlotButton.addActionListener(new plotTheFirstChannle_Button());


		PlotButton.setBounds(166, 11, 89, 23);
		//chart1.add(PlotButton);

		JButton refrechButton_1 = new JButton("refrechChart");
		refrechButton_1.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				refresh();

			}
		});

		refrechButton_1.setBounds(211, 43, 137, 40);
		chart1.add(refrechButton_1);


		forchart.setBounds(0, 130, 1700, 600);
		chart1.add(forchart);
		forchart.add(new ChartPanel(createChart(dataset,XPlot())), BorderLayout.CENTER);
		
		JButton ReadtoCSV = new JButton("Start Measumernt");


		ReadtoCSV.addActionListener(new StartMeasumernt_Button());

		ReadtoCSV.setBounds(10, 11, 133, 23);
		//chart1.add(ReadtoCSV);
		/*
		Start measurement button
		 */
		JButton clear = new JButton("Start Measumernt");
		clear.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				StartMeaseumnt_Windo x=new StartMeaseumnt_Windo();
				x.setVisible(true);

					try {


					} catch (Exception ex)   // any other error
					{
						JOptionPane.showMessageDialog(null, "Es gab einen Fehler!\n" + ex.getMessage());
					}
				}


		//	}
		});

		clear.setBounds(32, 43, 153, 40);
		chart1.add(clear);
		
		SamplingFrequenz = new JTextField();
		SamplingFrequenz.setBounds(633, 11, 102, 23);
		chart1.add(SamplingFrequenz);
		SamplingFrequenz.setColumns(10);
		
		JLabel Samplingfrequenzlsb = new JLabel("Samplingfrequenz");
		Samplingfrequenzlsb.setBounds(509, 11, 114, 23);
		chart1.add(Samplingfrequenzlsb);
		
		JLabel numberofsampleslab = new JLabel("period");
		numberofsampleslab.setBounds(764, 11, 121, 23);
		chart1.add(numberofsampleslab);

		number_of_samples = new JTextField();
		number_of_samples.setBounds(878, 11, 114, 21);
		chart1.add(number_of_samples);
		number_of_samples.setColumns(10);
		
		JButton ADCSampling = new JButton("Apply");
		ADCSampling.addActionListener(new Apply_Button());

		ADCSampling.setBounds(1020, 11, 89, 23);
		chart1.add(ADCSampling);
		SamlingFrequenz.setBounds(627, 51, 107, 24);
		chart1.add(SamlingFrequenz);


		SamlingFrequenz.addActionListener(new Frequenz_Selct_1_ComboBox());

		Period.setBounds(878, 51, 114, 24);
		chart1.add(Period);

		Period.addActionListener( new PeriodSelct_ComboBox());
		JLayeredPane layeredPane_2 = new JLayeredPane();
		tabbedPane.addTab("Excitation Settings", null, layeredPane_2, null);

		JTabbedPane tabbedPane_1 = new JTabbedPane(JTabbedPane.TOP);
		tabbedPane_1.setBounds(0, 0, 1700, 1008);
		layeredPane_2.add(tabbedPane_1);
		String[] items = {"PWM","Stop","Start"};
		// Get current value
		JComboBox Analoginput = new JComboBox(items);
		Analoginput.setBounds(92, 105, 89, 24);
		panel.add(Analoginput);


		ActionListener actionListener = new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {

				if (Analoginput.getSelectedItem() == "PWM") {
					analog = PWMCaluation();
				}

				if (Analoginput.getSelectedItem() == "Stop") {
					analog = "dis";
				}
				if (Analoginput.getSelectedItem() == "Start") {
					analog = "en";
				}
			}
		};

		Analoginput.addActionListener(actionListener);

		JLayeredPane PWM = new JLayeredPane();
		tabbedPane_1.addTab("Trapz", null, PWM, null);

		JLabel Ausgang_1 = new JLabel("Ausgang 1");
		Ausgang_1.setBounds(645, 0, 101, 29);
		PWM.add(Ausgang_1);

		JLabel Ausgang_2 = new JLabel("Ausgang 2");
		Ausgang_2.setBounds(934, 0, 85, 29);
		PWM.add(Ausgang_2);

		JPanel panel_2_2 = new JPanel();
		panel_2_2.setLayout(null);
		panel_2_2.setBounds(582, 32, 200, 189);
		PWM.add(panel_2_2);

		JLabel lblNewLabel_2_4 = new JLabel("Ausgabetyp");
		lblNewLabel_2_4.setBounds(10, 8, 63, 17);
		panel_2_2.add(lblNewLabel_2_4);

		JLabel Uon_Ion = new JLabel("Uon/Ion");
		Uon_Ion.setBounds(10, 30, 63, 17);
		panel_2_2.add(Uon_Ion);

		Volte_Value = new JTextField();
		Volte_Value.setColumns(10);
		Volte_Value.setBounds(78, 30, 96, 20);
		panel_2_2.add(Volte_Value);

		JLabel Ton = new JLabel("Ton");
		Ton.setBounds(10, 52, 63, 17);
		panel_2_2.add(Ton);

		Time_On = new JTextField();
		Time_On.setColumns(10);
		Time_On.setBounds(78, 52, 96, 20);
		panel_2_2.add(Time_On);

		JLabel Tin = new JLabel("Tin");
		Tin.setBounds(10, 74, 63, 17);
		panel_2_2.add(Tin);

		Time_In = new JTextField();
		Time_In.setColumns(10);
		Time_In.setBounds(78, 74, 96, 20);
		panel_2_2.add(Time_In);

		JComboBox comboBox_2 = new JComboBox();
		comboBox_2.setBounds(78, 4, 96, 24);
		panel_2_2.add(comboBox_2);

		JLabel Tout = new JLabel("Tout");
		Tout.setBounds(10, 98, 63, 17);
		panel_2_2.add(Tout);

		Time_Out = new JTextField();
		Time_Out.setColumns(10);
		Time_Out.setBounds(78, 96, 96, 20);
		panel_2_2.add(Time_Out);

		JLabel Tpeak = new JLabel("Tpeak");
		Tpeak.setBounds(10, 118, 63, 17);
		panel_2_2.add(Tpeak);

		Time_Peak = new JTextField();
		Time_Peak.setColumns(10);
		Time_Peak.setBounds(78, 118, 96, 20);
		panel_2_2.add(Time_Peak);

		JLabel Frequenz = new JLabel("Frequenz");
		Frequenz.setBounds(10, 140, 63, 17);
		panel_2_2.add(Frequenz);

		Frequency = new JTextField();
		Frequency.setColumns(10);
		Frequency.setBounds(78, 140, 96, 20);
		panel_2_2.add(Frequency);

		JLabel Anzahl = new JLabel("Anzahl");

		Anzahl.setBounds(10, 162, 63, 17);
		panel_2_2.add(Anzahl);

		Number_of_Pulses = new JTextField();
		Number_of_Pulses.setColumns(10);
		Number_of_Pulses.setBounds(78, 162, 96, 20);
		panel_2_2.add(Number_of_Pulses);

		JPanel panel_2_1_1 = new JPanel();
		panel_2_1_1.setLayout(null);
		panel_2_1_1.setBounds(872, 32, 200, 189);
		PWM.add(panel_2_1_1);

		JLabel lblNewLabel_2_3_1 = new JLabel("Ausgabetyp");
		lblNewLabel_2_3_1.setBounds(10, 8, 63, 17);
		panel_2_1_1.add(lblNewLabel_2_3_1);

		JLabel Uon_Ion2 = new JLabel("Uon/Ion");
		Uon_Ion2.setBounds(10, 30, 63, 17);
		panel_2_1_1.add(Uon_Ion2);

		Volte_Value_2 = new JTextField();
		Volte_Value_2.setColumns(10);
		Volte_Value_2.setBounds(78, 30, 96, 20);
		panel_2_1_1.add(Volte_Value_2);

		JLabel Ton2 = new JLabel("Ton");
		Ton2.setBounds(10, 52, 63, 17);
		panel_2_1_1.add(Ton2);

		Time_on_2 = new JTextField();
		Time_on_2.setColumns(10);
		Time_on_2.setBounds(78, 52, 96, 20);
		panel_2_1_1.add(Time_on_2);

		JLabel Tin2 = new JLabel("Tin");
		Tin2.setBounds(10, 74, 63, 17);
		panel_2_1_1.add(Tin2);

		Time_In_2 = new JTextField();
		Time_In_2.setColumns(10);
		Time_In_2.setBounds(78, 74, 96, 20);
		panel_2_1_1.add(Time_In_2);

		JComboBox comboBox_1_1 = new JComboBox();
		comboBox_1_1.setBounds(78, 4, 96, 24);
		panel_2_1_1.add(comboBox_1_1);

		JLabel Tout2 = new JLabel("Tout");
		Tout2.setBounds(10, 98, 63, 17);
		panel_2_1_1.add(Tout2);

		Time_Out_2 = new JTextField();
		Time_Out_2.setColumns(10);
		Time_Out_2.setBounds(78, 96, 96, 20);
		panel_2_1_1.add(Time_Out_2);

		JLabel Tpeak2 = new JLabel("Tpeak");
		Tpeak2.setBounds(10, 118, 63, 17);
		panel_2_1_1.add(Tpeak2);

		Time_Peak_2 = new JTextField();
		Time_Peak_2.setColumns(10);
		Time_Peak_2.setBounds(78, 118, 96, 20);
		panel_2_1_1.add(Time_Peak_2);

		JLabel Frequenz2 = new JLabel("Frequenz");
		Frequenz2.setBounds(10, 140, 63, 17);
		panel_2_1_1.add(Frequenz2);

		Frequency_2 = new JTextField();
		Frequency_2.setColumns(10);
		Frequency_2.setBounds(78, 140, 96, 20);
		panel_2_1_1.add(Frequency_2);

		JLabel Anzahl2 = new JLabel("Anzahl");
		Anzahl2.setBounds(10, 162, 63, 17);
		panel_2_1_1.add(Anzahl2);

		Number_of_Pulses_2 = new JTextField();
		Number_of_Pulses_2.setColumns(10);
		Number_of_Pulses_2.setBounds(78, 162, 96, 20);
		panel_2_1_1.add(Number_of_Pulses_2);

		OpenJson1.setBounds(638, 245, 108, 29);
		PWM.add(OpenJson1);
		this.OpenJson1.addActionListener(new Open_Jason_Button());

		SaveJason1.setBounds(638, 221, 108, 29);
		PWM.add(SaveJason1);
		this.SaveJason1.addActionListener(new savetojason());

		Foto = new JLabel("Foto");
		Foto.setBounds(0, -58, 1674, 1001);
		Foto.setIcon(imageTrapez);
		PWM.add(Foto);

		SaveJason2.setBounds(934, 221, 108, 29);
		PWM.add(SaveJason2);
		this.SaveJason2.addActionListener(new savetojason2());

		OpenJson2.setBounds(934, 248, 108, 29);
		PWM.add(OpenJson2);
		this.OpenJson2.addActionListener(new Open_Jason_Button());

		JLayeredPane Trapez = new JLayeredPane();
		tabbedPane_1.addTab("PWM", null, Trapez, null);
				
		JLabel lblNewLabel_1_2_1 = new JLabel("Ausgang 1");
		lblNewLabel_1_2_1.setBounds(638, 0, 85, 29);
		Trapez.add(lblNewLabel_1_2_1);
						
		JLabel lblNewLabel_1_1_1_1 = new JLabel("Ausgang 2");
		lblNewLabel_1_1_1_1.setBounds(938, 0, 85, 29);
		Trapez.add(lblNewLabel_1_1_1_1);
								
		JPanel Ausgang_PWM_1 = new JPanel();
		Ausgang_PWM_1.setLayout(null);
		Ausgang_PWM_1.setBounds(555, 32, 227, 189);
		Trapez.add(Ausgang_PWM_1);
														
		JLabel DutyCyclePWM1 = new JLabel("DutyCycle");
		DutyCyclePWM1.setBounds(10, 45, 63, 17);
		Ausgang_PWM_1.add(DutyCyclePWM1);
																
		JLabel FrequenzPWM1 = new JLabel("Frequenz [KHz]");
		FrequenzPWM1.setBounds(10, 112, 94, 17);
		Ausgang_PWM_1.add(FrequenzPWM1);
																		
		Frequenz_PWM_1 = new JTextField();
		Frequenz_PWM_1.setColumns(10);
		Frequenz_PWM_1.setBounds(114, 111, 88, 18);
		Ausgang_PWM_1.add(Frequenz_PWM_1);


		DutyCycle .addActionListener(new Duty_Cycle_1_ComboBox());
		DutyCycle.setBounds(114, 41, 98, 24);
		Ausgang_PWM_1.add(DutyCycle);
																								
		JPanel Ausgang_PWM_2 = new JPanel();
		Ausgang_PWM_2.setLayout(null);
		Ausgang_PWM_2.setBounds(864, 32, 237, 189);
		Trapez.add(Ausgang_PWM_2);
																																
		JLabel DutyCyclePWM2 = new JLabel("DutyCycle");
		DutyCyclePWM2.setBounds(10, 38, 63, 17);
		Ausgang_PWM_2.add(DutyCyclePWM2);

		JLabel FrequenzPWM2 = new JLabel("Frequenz [kHz]");
		FrequenzPWM2.setBounds(10, 111, 99, 17);
		Ausgang_PWM_2.add(FrequenzPWM2);

		Frequenz_PWM_2 = new JTextField();
		Frequenz_PWM_2.setColumns(10);
		Frequenz_PWM_2.setBounds(119, 108, 88, 20);
		Ausgang_PWM_2.add(Frequenz_PWM_2);
		
		
		DutyCycle_2.setBounds(119, 34, 98, 24);
		Ausgang_PWM_2.add(DutyCycle_2);
		DutyCycle_2.addActionListener(new Duty_Cycle_2_ComboBox());


		Start_PWM_1_Full_bridge.addActionListener(new Start_PWM_1_Full_bridge_Button());
		Start_PWM_1_Full_bridge.setBounds(545, 232, 127, 29);
		Trapez.add(Start_PWM_1_Full_bridge);


		Start_PWM_1_half_bridge.addActionListener(new Start_PWM_1_half_bridge_Button());
		Start_PWM_1_half_bridge.setBounds(680, 232, 127, 29);
		Trapez.add(Start_PWM_1_half_bridge);
		Start_PWM_2_Full_bridge.addActionListener(new Start_PWM_2_Full_bridge_Button());
		Start_PWM_2_Full_bridge.setBounds(845, 232, 127, 29);
		Trapez.add(Start_PWM_2_Full_bridge);

		Start_PWM_2_half_bridge.addActionListener(new Start_PWM_2_half_bridge_Button());

		Start_PWM_2_half_bridge.setBounds(995, 232, 127, 29);
		Trapez.add(Start_PWM_2_half_bridge);
		
		
		JLayeredPane Sprung = new JLayeredPane();
		tabbedPane_1.addTab("Sprung", null, Sprung, null);
				
		JPanel Ausgang_PWM_1_1 = new JPanel();
		Ausgang_PWM_1_1.setLayout(null);
		Ausgang_PWM_1_1.setBounds(513, 32, 269, 224);
		Sprung.add(Ausgang_PWM_1_1);
				
		JLabel Sprung_Dauer = new JLabel("Sprung Dauer");
		Sprung_Dauer.setHorizontalAlignment(SwingConstants.LEFT);
		Sprung_Dauer.setBounds(10, 60, 88, 20);
		Ausgang_PWM_1_1.add(Sprung_Dauer);
				
		JLabel FrequenzPWM1_1 = new JLabel("Anzahl of Sprung");
		FrequenzPWM1_1.setBounds(10, 112, 98, 20);
		Ausgang_PWM_1_1.add(FrequenzPWM1_1);
				
		Sprung_Number_1 = new JTextField();
		Sprung_Number_1.setColumns(10);
		Sprung_Number_1.setBounds(137, 111, 88, 21);
		Ausgang_PWM_1_1.add(Sprung_Number_1);
				

		Sprung_Dauer_1.addActionListener(new Sprung_1_Dauer_ComboBox());
		Sprung_Dauer_1.setBounds(127, 56, 98, 24);
		Ausgang_PWM_1_1.add(Sprung_Dauer_1);
				
		JLabel lblNewLabel_1_2_1_1 = new JLabel("Ausgang 1");
		lblNewLabel_1_2_1_1.setBounds(621, 0, 85, 29);
		Sprung.add(lblNewLabel_1_2_1_1);
				
		JLabel lblNewLabel_1_1_1_1_1 = new JLabel("Ausgang 2");
		lblNewLabel_1_1_1_1_1.setBounds(964, 0, 85, 29);
		Sprung.add(lblNewLabel_1_1_1_1_1);
				
		JPanel Ausgang_PWM_1_1_1 = new JPanel();
		Ausgang_PWM_1_1_1.setLayout(null);
		Ausgang_PWM_1_1_1.setBounds(880, 32, 269, 224);
		Sprung.add(Ausgang_PWM_1_1_1);
				
		JLabel Sprung_Dauer_1_1 = new JLabel("Sprung Dauer");
		Sprung_Dauer_1_1.setHorizontalAlignment(SwingConstants.LEFT);
		Sprung_Dauer_1_1.setBounds(10, 60, 88, 20);
		Ausgang_PWM_1_1_1.add(Sprung_Dauer_1_1);
				
		JLabel FrequenzPWM1_1_1 = new JLabel("Anzahl of Sprung");
		FrequenzPWM1_1_1.setBounds(10, 112, 98, 20);
		Ausgang_PWM_1_1_1.add(FrequenzPWM1_1_1);
				
		Sprung_Number_2 = new JTextField();
		Sprung_Number_2.setColumns(10);
		Sprung_Number_2.setBounds(137, 111, 88, 21);
		Ausgang_PWM_1_1_1.add(Sprung_Number_2);

		Sprung_Dauer_2.addActionListener(new Sprung_2_Dauer_ComboBox());
		Sprung_Dauer_2.setBounds(127, 56, 98, 24);
		Ausgang_PWM_1_1_1.add(Sprung_Dauer_2);
				
		JButton Sprung_1_Start = new JButton("Start Sprung ");
		Sprung_1_Start.setBounds(598, 279, 111, 29);
		Sprung.add(Sprung_1_Start);
		Sprung_1_Start.addActionListener(new Sprung_1_Button());
				
		JButton Sprung_2_Start = new JButton("Start Sprung ");
		Sprung_2_Start.setBounds(964, 282, 111, 29);
		Sprung.add(Sprung_2_Start);
		Sprung_2_Start.addActionListener(new Sprung_2_Button());
		JLayeredPane layeredPane_1 = new JLayeredPane();
		tabbedPane.addTab("Measurment Settings", null, layeredPane_1, null);
				
		JPanel panel_1 = new JPanel();
		panel_1.setBounds(10, 0, 502, 316);
		layeredPane_1.add(panel_1);

		panel_1.setLayout(null);
		JLabel lblNewLabel = new JLabel("Current");
		lblNewLabel.setBounds(10, 26, 47, 14);
		panel_1.add(lblNewLabel);
		JLabel lblVoltage = new JLabel("Voltage");
		lblVoltage.setBounds(10, 55, 47, 14);
		panel_1.add(lblVoltage);
		JLabel lblPosition = new JLabel("Position");
		lblPosition.setBounds(10, 79, 47, 26);
		panel_1.add(lblPosition);
		JTextPane textPane = new JTextPane();
		textPane.setBounds(62, 21, 79, 25);
		panel_1.add(textPane);
						
		JTextPane textPane_1 = new JTextPane();
		textPane_1.setBounds(62, 51, 79, 25);
		panel_1.add(textPane_1);
								
		JTextPane textPane_2 = new JTextPane();
		textPane_2.setBounds(62, 81, 79, 25);
		panel_1.add(textPane_2);





	}
/*
Funktion zum Erstellen eines Diagramms
 */
	public static JFreeChart createChart(XYDataset dataset, String xAxisLabel) {
		String chartTitle = "Analog Messung";

		//String xAxisLabel = "Zeit in "+XPlot();
		String yAxisLabel = "Spannung (volt)";


		JFreeChart chart = ChartFactory.createXYLineChart(chartTitle,
				xAxisLabel, yAxisLabel, dataset, PlotOrientation.VERTICAL, true, true, false);

		XYPlot plot = chart.getXYPlot();


		NumberAxis xAxis = new NumberAxis();

		xAxis.setLabel(xAxisLabel);
		NumberAxis yAxis = (NumberAxis) plot.getRangeAxis();
		yAxis.setRange(-10, 10);
		yAxis.setTickUnit(new NumberTickUnit(0.5));







		return chart;
	}


	private void connect() {

	}

	@Override
	public void update(Observable o, Object arg) {

	}
/*
zur Berechnung in korrekter Zahlenform Frequenz und Duty Cycle
 */
	String PWMCaluation() {
		//168=1us
		String Massg = null;
		try {


			double frequenz_pwm_1Value = Double.parseDouble(Frequenz_PWM_1.getText());
			double duty_cycle_pwm_1_Value = Duty_Cycle_PWM_1;
			double peroid_pwm_1Value = 1 / frequenz_pwm_1Value;

			int peroid_pwm_1Valuetimer = (int) (peroid_pwm_1Value * 168 *33* 10);
			//int duty_cycle_pwm_1_percent = (int) (((duty_cycle_pwm_1_Value) / 100) * peroid_pwm_1Valuetimer);
			double duty_cycle_pwm_1_percent =  ((duty_cycle_pwm_1_Value)  * peroid_pwm_1Valuetimer);
			System.out.println(duty_cycle_pwm_1_percent);
			Massg = "2/" + String.valueOf(peroid_pwm_1Valuetimer) + "/" + String.valueOf((int)duty_cycle_pwm_1_percent);
			return Massg;

		} catch (Exception NumberFormatException) {
			JOptionPane.showMessageDialog(null, "Bitte Geben Sie die Frequenz und Duty Cycle  \n" );
		}
		return Massg;
	}







/*
die Anzahl der verfügbaren Anschlüsse zu ermitteln
 */

	int Protzahl() {
		return portNames.length;
	}

	public static String fileToString(String filePath) throws Exception {
		String input = null;
		Scanner sc = new Scanner(new File(filePath));
		StringBuffer sb = new StringBuffer();
		while (sc.hasNextLine()) {
			input = sc.nextLine();
			sb.append(input);
		}
		return sb.toString();


/*
löschen
das Digramm
 */
	}
	void refresh(){
		series1.clear();
		series3.clear();
		series2.clear();

	}
/*
 X-Beschriftung anpassen
 */
	static String XPlot(){
		String Xachselabe=String.valueOf(sampledauer);
		String Periodselction= String.valueOf(Period.getSelectedItem());
		String x=Periodselction;
		return x+"   [ein Sample daurt"+Xachselabe+" uS] ";
	}








}

