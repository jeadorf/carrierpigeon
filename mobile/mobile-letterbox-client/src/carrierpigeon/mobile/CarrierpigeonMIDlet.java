package carrierpigeon.mobile;

import java.io.IOException;
import java.io.OutputStream;
import javax.microedition.io.Connector;
import javax.microedition.io.StreamConnection;
import javax.microedition.lcdui.Display;
import javax.microedition.midlet.MIDlet;

/**
 * This MIDlet is a carrierpigeon client. It lets users of cell phones or
 * other portable devices send messages to a letterbox server. It features
 * auto-discovery of bluetooth devices in the vicinity, T9 with most devices,
 * and basic feedback on failure.
 *
 * @see MessagePanel
 * @see WaitPanel
 * @see SendPanel
 * @see ErrorPanel
 */
public class CarrierpigeonMIDlet extends MIDlet {

    private ErrorPanel errorPanel;

    private MessagePanel messagePanel;

    private SendPanel sendPanel;

    private WaitPanel waitPanel;

    private DeviceDetector deviceDetector;

    /**
     * Initializes the MIDlet. This will also trigger device detection, such
     * that the user does not have to wait when s/he finishs typing the message.
     * 
     * The error panel is initialized at the very first possible moment, such
     * that it can be shown to the user even if the initialization of other
     * panels and services fails.
     */
    public void startApp() {
        Display display = Display.getDisplay(this);

        // must go first
        errorPanel = new ErrorPanel(this);

        try {
            // Detect whether the MIDlet runs in the emulator on a workstation
            // or on a real phone. Reading system properties might be an action
            // that is guarded by some security manager implementation. This
            // switch may not break the application when running on a mobile.
            try {
                if (System.getProperty("carrierpigeon.emulator") != null) {
                    deviceDetector = new MockDeviceDetector();
                }
            } finally {
                if (deviceDetector == null) {
                    deviceDetector = new DefaultDeviceDetector();
                }
            }

            deviceDetector.startDiscovery();

            messagePanel = new MessagePanel(this);
            sendPanel = new SendPanel(this);
            waitPanel = new WaitPanel(this);

            display.setCurrent(messagePanel);
        } catch (Exception e) {
            errorPanel.setException(e);
            display.setCurrent(errorPanel);
        }
    }

    public void pauseApp() {
    }

    public void destroyApp(boolean unconditional) {
        notifyDestroyed();
    }

    public void sendMessage(String message, String btAddress) throws IOException {
        StreamConnection conn = null;
        try {
            String serviceUrl = "btspp://" + btAddress + ":1";
            conn = (StreamConnection) Connector.open(serviceUrl, Connector.WRITE);
            // TODO: find out why the calls to sleep are necessary
            Thread.sleep(500);
            OutputStream out = conn.openOutputStream();
            out.write('\r');
            out.write('\n');
            out.write(encode(message));
            out.write('\r');
            out.write('\n');
            out.flush();
            Thread.sleep(1500);
        } catch (InterruptedException ex) {
            throw new IOException("Got interrupted.");
        } finally {
            if (conn != null) {
                conn.close();
            }
        }
    }

    /**
     * Cares about encoding a message into bytes. The letterbox does not
     * understand umlauts or any special characters, it only supports parts of
     * the ASCII alphabet. Characters that cannot be displayed on the letterbox
     * are sent anyway, but never two bytes for one character.
     *
     * @param message
     * @return
     */
    byte[] encode(String message) {
        return message.getBytes();
    }

    public DeviceDetector getDeviceDetector() {
        return deviceDetector;
    }

    public ErrorPanel getErrorPanel() {
        return errorPanel;
    }

    public MessagePanel getMessagePanel() {
        return messagePanel;
    }

    public SendPanel getSendPanel() {
        return sendPanel;
    }

    public WaitPanel getWaitPanel() {
        return waitPanel;
    }

}
