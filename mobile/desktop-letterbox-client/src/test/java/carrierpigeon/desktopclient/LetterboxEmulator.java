package carrierpigeon.desktopclient;

import java.io.DataOutputStream;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.bluetooth.UUID;
import javax.microedition.io.Connector;
import javax.microedition.io.StreamConnection;
import javax.microedition.io.StreamConnectionNotifier;
import junit.framework.Assert;

/**
 * Emulates the AVR letterbox server.
 *
 * @author Julius Adorf
 */
public class LetterboxEmulator implements Runnable {

    public static final UUID uuid = new UUID(0x2208); // number is random

    public static final String GREETING = "hello from server";
    
    /**
     * Accepts one single connection to a client and writes
     * a greeting message.
     */
    public void run() {
        StreamConnectionNotifier service = null;
        try {
            String serviceUrl = String.format("btspp://localhost:%s;name=TServer", uuid);
            service = (StreamConnectionNotifier) Connector.open(serviceUrl);
            StreamConnection connection = (StreamConnection) service.acceptAndOpen();
            DataOutputStream out = connection.openDataOutputStream();
            out.writeUTF(GREETING);
            out.close();
            connection.close();
        } catch (IOException ex) {
            Assert.fail(ex.toString());
        } finally {
            if (service != null) {
                try {
                    service.close();
                } catch (IOException ex) {
                    Assert.fail(ex.toString());
                }
            }
        }
    }
}
