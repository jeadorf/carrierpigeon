package carrierpigeon.desktopclient;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import javax.microedition.io.Connector;
import javax.microedition.io.StreamConnection;

/**
 * Derived from the minimal example found at
 * http://bluecove.org/bluecove/apidocs/index.html
 */
public class DesktopLetterboxClient {

    public static void main(String[] args) throws Exception {
        // btm-222 : 00126F037095
        // mopple1 : 0009DD504D97
        // halman: 001BDC0F5FC9
        StreamConnection conn = (StreamConnection) Connector.open("btspp://001BDC0F5FC9:1;master=false;authenticate=false;encrypt=false");
        DataOutputStream out = conn.openDataOutputStream();
        DataInputStream in = conn.openDataInputStream();

        System.out.println("waiting for greeting ...");
        String greeting = in.readUTF();
        System.out.println("recveived " + greeting);
        System.out.println("writing message");
        out.writeUTF("Nachricht.");
        out.flush();
        System.out.println("waiting for confirmation");
        String confirmation = in.readUTF();
        System.out.println("received " + confirmation);

        out.close();
        conn.close();
    }

}
