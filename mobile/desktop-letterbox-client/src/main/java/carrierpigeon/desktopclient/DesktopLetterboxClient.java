package carrierpigeon.desktopclient;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import javax.microedition.io.Connector;
import javax.microedition.io.StreamConnection;

/**
 * Derived from the minimal example found at
 * http://bluecove.org/bluecove/apidocs/index.html
 */
public class DesktopLetterboxClient {

    /**
     * Send a message to the letterbox server.
     * 
     * @param serviceUrl
     * @param message
     * @param log collection parameter
     */
    public void sendMessage(String serviceUrl, String message) throws
            IOException {
        // btm-222 : 00126F037095
        // mopple1 : 0009DD504D97
        // halman: 001BDC0F5FC9
        System.out.printf("[client] connecting to '%s'\n", serviceUrl);
        StreamConnection conn = (StreamConnection) Connector.open(serviceUrl);
        DataOutputStream out = conn.openDataOutputStream();
        DataInputStream in = conn.openDataInputStream();

        System.out.println("[client] waiting for greeting ...");
        // log.add(LetterboxProtocol.clientExpectsGreeting);
        String greeting = in.readUTF();
        System.out.printf("[client] received '%s'\n", greeting);
        // log.add(LetterboxProtocol.clientGetsGreeting);
        System.out.printf("[client] writing message '%s'\n", message);
        // log.add(LetterboxProtocol.clientSendsMessage);
        out.writeUTF(message);
        out.flush();
        System.out.println("[client] waiting for confirmation ...");
        // log.add(LetterboxProtocol.clientExpectsConfirmation);
        String confirmation = in.readUTF();
        System.out.printf("[client] received confirmation '%s'\n", confirmation);
        // log.add(LetterboxProtocol.clientGetsConfirmation);
        System.out.println("[client] closing connection");
        // log.add(LetterboxProtocol.clientClosesConnection);
        in.close();
        out.close();
        conn.close();
    }

}
