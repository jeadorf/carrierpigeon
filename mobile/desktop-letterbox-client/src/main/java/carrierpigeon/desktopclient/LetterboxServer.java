package carrierpigeon.desktopclient;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.util.ArrayList;
import java.util.List;
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

    public static final UUID uuid = new UUID("1101", true);

    public static final String GREETING = "HI";

    public static final String CONFIRM = "OK";

    private boolean error = false;

    private List<LetterboxProtocol> protocolLog;

    // see http://www.jsr82.com/jsr-82-sample-spp-server-and-client

    /**
     * Accepts one single connection to a client and writes
     * a greeting message.
     */
    public void run() {
        protocolLog = new ArrayList();
        StreamConnectionNotifier service = null;
        try {
            String serviceUrl = String.format("btspp://localhost:%s;name=letterbox-server;authenticate=false;encrypt=false;", uuid);
            System.out.printf("[server] init server on '%s'\n", serviceUrl);
            service = (StreamConnectionNotifier) Connector.open(serviceUrl);
            System.out.println("[server] waiting for connection ...");
            StreamConnection connection = (StreamConnection) service.acceptAndOpen();
            System.out.println("[server] accepted connection");

            DataOutputStream out = connection.openDataOutputStream();
            DataInputStream in = connection.openDataInputStream();
            // handshake
            System.out.printf("[server] writing greeting '%s'\n", GREETING);
            protocolLog.add(LetterboxProtocol.serverSendsGreeting);
            out.writeUTF(GREETING);
            // receive message
            System.out.println("[server] waiting for message ...");
            protocolLog.add(LetterboxProtocol.serverExpectsMessage);
            String message = in.readUTF();
            System.out.printf("[server] received message '%s'\n", message);
            protocolLog.add(LetterboxProtocol.serverGetsMessage);
            // confirm
            System.out.printf("[server] writing confirmation '%s'\n", CONFIRM);
            protocolLog.add(LetterboxProtocol.serverSendsConfirmation);
            out.writeUTF(CONFIRM);
            System.out.println("[server] closing connection");
            protocolLog.add(LetterboxProtocol.serverClosesConnection);
            // TODO: is it necessary to close data streams before closing connection?
            out.close();
            in.close();
            connection.close();
        } catch (Exception ex) {
            error = true;
            ex.printStackTrace();
        } finally {
            if (service != null) {
                try {
                    service.close();
                } catch (Exception ex) {
                    error = true;
                    ex.printStackTrace();
                }
            }
        }
    }

    public boolean isError() {
        return error;
    }

    public List<LetterboxProtocol> getProtocolLog() {
        return protocolLog;
    }

    public static void main(String[] args) {
        System.out.println("Running server...");
        LetterboxServer server = new LetterboxServer();
        server.run();
    }

}
