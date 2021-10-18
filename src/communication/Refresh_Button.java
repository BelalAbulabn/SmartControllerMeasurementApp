package communication;

import com.fazecast.jSerialComm.SerialPort;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import static gui.Windo.Portlist;
/*
Diese Klasse ist für die Aktualisierung der Liste aller verfügbaren COM-Ports zuständig.
 */
public class Refresh_Button implements ActionListener {
    public Refresh_Button() {
    }
   String []Ports;
    SerialPort[] portNames = SerialPort.getCommPorts();
    @Override
    public void actionPerformed(ActionEvent e) {
        deletallports();
        updateallports();
    }
    void deletallports() {
        Portlist.removeAllItems();

    }
    void updateallports() {

        for (int i = 0; i < Protzahl(); i++) {
            Portlist.addItem(portNames[i].getSystemPortName());

        }
    }
    String[] findetheport() {
        for (int i = 0; i < portNames.length; i++) {

            Ports[i]=portNames[i].getSystemPortName();


        }return Ports;
    }
   int Protzahl() {
        return portNames.length;
    }

}