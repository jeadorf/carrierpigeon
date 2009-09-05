package carrierpigeon.desktopclient;

import com.intel.bluetooth.EmulatorTestsHelper;
import java.io.DataInputStream;
import javax.bluetooth.DiscoveryAgent;
import javax.bluetooth.LocalDevice;
import javax.bluetooth.ServiceRecord;
import javax.microedition.io.Connector;
import javax.microedition.io.StreamConnection;
import org.junit.Before;
import org.junit.Test;
import org.junit.After;
import static org.junit.Assert.*;

/**
 *
 * @author julius
 */
public class LetterboxConnectionTest {

    private Thread serverThread;
    private LetterboxServer letterboxServer;

    @Before
    public void setUp() throws Exception {
        EmulatorTestsHelper.startInProcessServer();
        EmulatorTestsHelper.useThreadLocalEmulator();
        letterboxServer = new LetterboxServer();
        serverThread = EmulatorTestsHelper.runNewEmulatorStack(letterboxServer);
    }

    @Test
    public void testConnect() throws Exception { // TODO: finalize correctly
        LocalDevice localDev = LocalDevice.getLocalDevice();
        DiscoveryAgent agent = localDev.getDiscoveryAgent();
        String serviceUrl = agent.selectService(LetterboxServer.uuid,
                ServiceRecord.NOAUTHENTICATE_NOENCRYPT, false);
        StreamConnection conn = (StreamConnection) Connector.open(serviceUrl);
        DataInputStream in = conn.openDataInputStream();
        assertEquals("expect greeting", LetterboxServer.GREETING, in.readUTF());
        assertFalse("error from letterbox emulator", letterboxServer.isError());
        in.close();
    }

    @After
    public void tearDown() throws Exception {
        if ((serverThread != null) && (serverThread.isAlive())) {
            serverThread.interrupt();
            serverThread.join();
        }
        EmulatorTestsHelper.stopInProcessServer();
    }
    
}
