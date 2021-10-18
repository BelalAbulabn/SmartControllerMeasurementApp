package communication;

import com.fazecast.jSerialComm.SerialPort;

import java.util.ArrayList;
import java.util.List;

public class Findetheport_Funktion {

	static String []Ports;
	public static SerialPort[] portNames = SerialPort.getCommPorts();

	public static String[] Findetheport() {
		for (int i = 0; i < portNames.length; i++) {

			Ports[i]=portNames[i].getSystemPortName();


		}return Ports;
	}


	public static int protzahl() {
		return portNames.length;
	}
}


