package carrierpigeon.mobile;

import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.Gauge;
import javax.microedition.lcdui.Item;
import javax.microedition.lcdui.StringItem;

/**
 * Shown until the bluetooth discovery service completes.
 */
public class WaitPanel extends Form implements CommandListener {

    private CarrierpigeonMIDlet midlet;

    public WaitPanel(CarrierpigeonMIDlet midlet) {
        super("Searching bluetooth devices");
        this.midlet = midlet;

        Gauge gauge = new Gauge("Scanning for bluetooth devices", false,
                Gauge.INDEFINITE, Gauge.CONTINUOUS_RUNNING);
        append(gauge);
        StringItem about = new StringItem("Carrierpigeon",
                "Trying to discover receivers! This may take some time...",
                Item.HYPERLINK);
        append(about);

        addCommand(new Command("Exit", Command.EXIT, 1));
        setCommandListener(this);
    }

    public void commandAction(Command c, Displayable d) {
        if (c.getCommandType() == Command.EXIT) {
            midlet.destroyApp(true);
        }
    }
}
