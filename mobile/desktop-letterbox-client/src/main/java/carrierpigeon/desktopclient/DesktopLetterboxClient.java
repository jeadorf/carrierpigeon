package carrierpigeon.desktopclient;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.util.Vector;
import javax.bluetooth.DeviceClass;
import javax.bluetooth.DiscoveryAgent;
import javax.bluetooth.DiscoveryListener;
import javax.bluetooth.LocalDevice;
import javax.bluetooth.RemoteDevice;
import javax.bluetooth.ServiceRecord;
import javax.microedition.io.Connector;
import javax.microedition.io.StreamConnection;

/**
 * Derived from the minimal example found at
 * http://bluecove.org/bluecove/apidocs/index.html
 */
public class DesktopLetterboxClient {

    public static final Vector discoveredDevices = new Vector();

    public static void main(String[] args) throws IOException, InterruptedException {
        final Object inquiryCompletedEvent = new Object();

        discoveredDevices.clear();

        DiscoveryListener listener = new DiscoveryListener() {

            public void deviceDiscovered(RemoteDevice btDevice, DeviceClass cod) {
                System.out.println("Device " + btDevice.getBluetoothAddress() + " found");
                discoveredDevices.addElement(btDevice);
                try {
                    System.out.println("     name " + btDevice.getFriendlyName(false));
                } catch (IOException ioe) {
                    ioe.printStackTrace();
                }
            }

            public void inquiryCompleted(int discType) {
                System.out.println("Device Inquiry completed!");
                synchronized(inquiryCompletedEvent){
                    inquiryCompletedEvent.notifyAll();
                }
            }

            public void serviceSearchCompleted(int transID, int respCode) {
            }

            public void servicesDiscovered(int transID, ServiceRecord[] servRecord) {
            }
        };

        synchronized(inquiryCompletedEvent) {
            boolean started = LocalDevice.getLocalDevice().getDiscoveryAgent().startInquiry(DiscoveryAgent.GIAC, listener);
            if (started) {
                System.out.println("wait for device inquiry to complete...");
                inquiryCompletedEvent.wait();
                System.out.println(discoveredDevices.size() +  " device(s) found");
            }
        }


        // next try to connect directly to btm-222
        StreamConnection conn = (StreamConnection) Connector.open("btspp://00126F037095:1;master=false;authenticate=false;encrypt=false");
        DataOutputStream out = conn.openDataOutputStream();
        DataInputStream in = conn.openDataInputStream();

        // working?
        System.out.println("Got answer from bluetooth device: " + in.readUTF());
        out.writeUTF("Hello!");

        out.close();
        conn.close();
    }

}
