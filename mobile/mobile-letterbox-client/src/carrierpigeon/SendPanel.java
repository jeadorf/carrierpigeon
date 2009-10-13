package carrierpigeon;

import java.io.IOException;
import java.io.OutputStream;
import java.util.Hashtable;
import java.util.Vector;
import javax.bluetooth.BluetoothStateException;
import javax.bluetooth.RemoteDevice;
import javax.microedition.io.Connector;
import javax.microedition.io.StreamConnection;
import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.List;

/**
 *
 * @author julius
 */
public class SendPanel extends List implements CommandListener, DeviceDetector.CompletionListener {

    private CarrierpigeonMIDlet midlet;

    /**
     * Maps device names to bluetooth addresses.
     */
    private Hashtable btAddresses;

    public SendPanel(CarrierpigeonMIDlet midlet) throws BluetoothStateException {
        super("Select receiver", IMPLICIT);

        this.midlet = midlet;
        this.btAddresses = new Hashtable();

        addCommand(new Command("Send", Command.OK, 1));
        setCommandListener(this);

        // Start device discovery early
        midlet.getDeviceDetector().addCompletionListener(this);
    }

    public void deviceDetectionComplete() {
        Display.getDisplay(midlet).callSerially(new Runnable() {

            public void run() {
                Vector devices = midlet.getDeviceDetector().getDevices();
                for (int i = 0; i <
                        devices.size(); i++) {
                    RemoteDevice dev = (RemoteDevice) devices.elementAt(i);
                    String name;
                    String btAddr = dev.getBluetoothAddress();
                    try {
                        name = dev.getFriendlyName(true);
                        if (name == null) {
                            name = btAddr;
                        }
                    } catch (IOException ex) {
                        name = btAddr;
                    }

                    btAddresses.put(name, btAddr);
                    append(name, null);
                }
            }
        });
    }

    public void commandAction(Command c, Displayable d) {
        if (c.getCommandType() == Command.OK) {
            // When the MIDlet tries to open a bluetooth connection, a dialog
            // will pop up where the user must confirm that it is allowed to do
            // so. This dialog might get blocked by the connection request,
            // resulting in a deadlock. So we cannot use callSerially but rather
            // invoke the sendMessage method from within a separate thread.
            new Thread(new Runnable() {

                public void run() {
                    sendMessage();
                }
            }).start();
        }

    }

    // TODO: deal with umlauts
    private void sendMessage() {
        try {
            // String serviceUrl = "btspp://00126F037095:1"; // eumel
            String selectedDeviceName = getString(getSelectedIndex());
            String serviceUrl = "btspp://" + btAddresses.get(selectedDeviceName) + ":1";

            StreamConnection conn = (StreamConnection) Connector.open(serviceUrl, Connector.WRITE);

            // TODO: find out why the calls to sleep are necessary
            Thread.sleep(500);
            OutputStream out = conn.openOutputStream();

            String message = midlet.getMessagePanel().getMessage();
            byte[] messageBuffer = ("\r\n" + message + "\r\n").getBytes();
            out.write(messageBuffer);
            out.flush();
            conn.close();

            Thread.sleep(1500);

            // TODO: the connection does not seem to be closed by the call to
            //       conn.close() - but as soon as the program quits, the
            //       mobile disconnects.
            midlet.destroyApp(true);
        } catch (Exception e) {
            e.printStackTrace();
            ErrorPanel errorPanel = midlet.getErrorPanel();
            errorPanel.setException(e);
            Display.getDisplay(midlet).setCurrent(errorPanel);
        }
    }
}
