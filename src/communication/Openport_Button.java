package communication;

import com.fazecast.jSerialComm.SerialPort;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import static gui.Windo.*;
/*
 Diese Klasse ist für die Kommunikation mit dem COM-Port die Konfiguration
 ,sowie der UART-Kommunikation mit dem ausgewählten Port zuständig.

 */
public class Openport_Button implements ActionListener {
    @Override
    public void actionPerformed(ActionEvent e) {
        if (ConnectButton.getText().equals("Connect")) {
            opentheselctedport();
            if (chosenPort.openPort()) {
                ConnectButton.setText("Disconnect");
                Portlist.setEnabled(false);
            }
        } else {
            // disconnect from the serial port
            chosenPort.closePort();
            Portlist.setEnabled(true);
            ConnectButton.setText("Connect");
        }
    }

    void opentheselctedport() {

        chosenPort = SerialPort.getCommPort(Portlist.getSelectedItem().toString());
        chosenPort.setComPortTimeouts(SerialPort.TIMEOUT_SCANNER, 0, 0);
        chosenPort.setBaudRate(115200);
        chosenPort.setParity(0);
        chosenPort.setNumDataBits(8);
        chosenPort.setNumStopBits(1);

    }
    }


