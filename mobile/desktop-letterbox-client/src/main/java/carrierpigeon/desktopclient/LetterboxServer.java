package carrierpigeon.desktopclient;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import javax.bluetooth.UUID;
import javax.microedition.io.Connector;
import javax.microedition.io.StreamConnection;
import javax.microedition.io.StreamConnectionNotifier;

/**
 * Emulates the AVR letterbox server.
 *
 * @author Julius Adorf
 */
public class LetterboxServer implements Runnable {

    public static final UUID uuid = new UUID(0x2208); // number is random

    public static final String GREETING = "HI";

    public static final String CONFIRM = "OK";

    private boolean error = false;

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
            DataInputStream in = connection.openDataInputStream();
            // handshake
            out.writeUTF(GREETING);
            // receive message
            in.readUTF();
            // confirm
            out.writeUTF(CONFIRM);
            out.close();
            connection.close();
        } catch (IOException ex) {
            error = true;
            ex.printStackTrace();
        } finally {
            if (service != null) {
                try {
                    service.close();
                } catch (IOException ex) {
                    error = true;
                    ex.printStackTrace();
                }
            }
        }
    }

    public boolean isError() {
        return error;
    }

    public static void main(String[] args) {
        System.out.println("Running server...");
        LetterboxServer em = new LetterboxServer();
        em.run();
    }

}
