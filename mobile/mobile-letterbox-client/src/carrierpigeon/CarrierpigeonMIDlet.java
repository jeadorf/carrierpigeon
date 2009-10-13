package carrierpigeon;

import javax.microedition.lcdui.Display;
import javax.microedition.midlet.MIDlet;

/**
 *
 */
public class CarrierpigeonMIDlet extends MIDlet {

    public void startApp() {
        Display display = Display.getDisplay(this);
        display.setCurrent(new MessagePanel(this));
    }

    public void pauseApp() {
    }

    public void destroyApp(boolean unconditional) {
        notifyDestroyed();
    }

}
