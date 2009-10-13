package carrierpigeon;

import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.StringItem;

/**
 *
 * @author julius
 */
public class ErrorPanel extends Form implements CommandListener {

    private final CarrierpigeonMIDlet midlet;

    public ErrorPanel(CarrierpigeonMIDlet midlet) {
        super("Error!");
        this.midlet = midlet;
     
        addCommand(new Command("Exit", Command.EXIT, 1));
        setCommandListener(this);
    }

    public void setException(Exception e) {
        append(new StringItem("An error occurred.", e.getMessage()));
    }

    public void commandAction(Command c, Displayable d) {
        midlet.destroyApp(true);
    }

}
