package carrierpigeon.mobile;

import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.TextField;

/**
 * Shows a text field that lets the user enter the message text. 
 */
public class MessagePanel extends Form implements CommandListener {

    private CarrierpigeonMIDlet midlet;

    private TextField messageTextField;

    public MessagePanel(CarrierpigeonMIDlet midlet) {
        super("Carrierpigeon client");
        this.midlet = midlet;
        this.messageTextField = new TextField("Message text",
                "0123456789" +
                "0123456789" +
                "0123456789" +
                "0123456789" +
                "0123456789" +
                "0123456789" +
                "0123456789" +
                "0123456789" +
                "0123456789" +
                "0123456789" +
                "0123456789", 112, TextField.ANY);
        messageTextField.setInitialInputMode("UCB_BASIC_LATIN");
        append(messageTextField);

        setCommandListener(this);
        addCommand(new Command("Send", Command.OK, 1));
        addCommand(new Command("Exit", Command.EXIT, 1));
    }

    public String getMessage() {
        return messageTextField.getString();
    }

    public void commandAction(Command command, Displayable displayable) {
        if (command.getCommandType() == Command.EXIT) {
            midlet.destroyApp(true);
        } else if (command.getCommandType() == Command.OK) {
            final Display display = Display.getDisplay(midlet);
            display.setCurrent(midlet.getWaitPanel());
            display.callSerially(new Runnable() {

                public void run() {
                    try {
                        midlet.getDeviceDetector().waitForCompletion();
                        display.setCurrent(midlet.getSendPanel());
                    } catch (InterruptedException ex) {
                        ErrorPanel errorPanel = midlet.getErrorPanel();
                        display.setCurrent(errorPanel);
                    }
                }
            });
        }
    }
}
