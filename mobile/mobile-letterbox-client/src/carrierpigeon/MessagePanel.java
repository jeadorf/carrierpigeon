package carrierpigeon;

import java.io.OutputStream;
import javax.microedition.io.Connector;
import javax.microedition.io.StreamConnection;
import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.TextField;

/**
 *
 */
public class MessagePanel extends Form implements CommandListener {

    private CarrierpigeonMIDlet midlet;

    private TextField messageTextField;

    public MessagePanel(CarrierpigeonMIDlet midlet) {
        super("Carrierpigeon client");
        this.midlet = midlet;
        this.messageTextField = new TextField("Message text", "type message here", 112, TextField.ANY);
        append(messageTextField);
        try {
            // Set up this form to listen to command events
            setCommandListener(this);
            // Add the Curreencies command
            addCommand(new Command("Send", Command.OK, 1));
            // Add the Exit command
            addCommand(new Command("Exit", Command.EXIT, 1));
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void commandAction(Command command, Displayable displayable) {
        if (command.getCommandType() == Command.EXIT) {
            midlet.destroyApp(true);
        } else if (command.getCommandType() == Command.OK) {

            new Thread(new Runnable() {

                public void run() {
                    sendMessage();
                    midlet.destroyApp(true);
                }
            }).start();


        }
    }

    public void sendMessage() {
        try {
            String serviceUrl = "btspp://00126F037095:1"; // eumel
            // String serviceUrl = "btspp://0009DD504D97:1"; // mopple-2

            StreamConnection conn = (StreamConnection) Connector.open(serviceUrl, Connector.WRITE);

            Thread.sleep(500);
            OutputStream out = conn.openOutputStream();

            byte[] message = ("\r\n" + messageTextField.getString() + "\r\n").getBytes();
            out.write(message);
            out.flush();
            conn.close();
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println(e.getMessage());
        }
    }
}
