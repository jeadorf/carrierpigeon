import javax.bluetooth.DeviceClass;
import javax.bluetooth.DiscoveryAgent;
import javax.bluetooth.DiscoveryListener;
import javax.bluetooth.LocalDevice;
import javax.bluetooth.RemoteDevice;
import javax.bluetooth.ServiceRecord;

/**
 * quick case study
 * 
 * @author julius
 */
public class BluetoothDiscovery {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) throws Exception {
        LocalDevice ld = LocalDevice.getLocalDevice();
        DiscoveryAgent agent = ld.getDiscoveryAgent();
        agent.startInquiry(DiscoveryAgent.LIAC, new DiscoveryListener() {
            public void deviceDiscovered(RemoteDevice device, DeviceClass arg1) {
                System.out.println("deviceDiscovered: " + device.getBluetoothAddress());
            }
            public void servicesDiscovered(int arg0, ServiceRecord[] arg1) {
                System.out.println("servicesDiscovered");
            }

            public void serviceSearchCompleted(int arg0, int arg1) {
                System.out.println("serviceSearchCompleted");
            }

            public void inquiryCompleted(int arg0) {
                System.out.println("inquiryCompleted");
            }
        });
    }
}
