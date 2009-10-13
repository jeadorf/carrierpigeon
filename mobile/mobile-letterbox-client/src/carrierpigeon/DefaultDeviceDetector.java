package carrierpigeon;

import java.io.IOException;
import javax.bluetooth.BluetoothStateException;
import javax.bluetooth.DeviceClass;
import javax.bluetooth.DiscoveryAgent;
import javax.bluetooth.DiscoveryListener;
import javax.bluetooth.LocalDevice;
import javax.bluetooth.RemoteDevice;
import javax.bluetooth.ServiceRecord;

/**
 *
 * @author julius
 */
public class DefaultDeviceDetector extends DeviceDetector {

    private boolean started = false;

    public synchronized void startDiscovery() throws BluetoothStateException {
        DiscoveryListener listener = new DiscoveryListener() {

            public void deviceDiscovered(RemoteDevice btDevice, DeviceClass cod) {
                System.out.println("Device " + btDevice.getBluetoothAddress() + " found");
                devices.addElement(btDevice);
                try {
                    System.out.println("     name " + btDevice.getFriendlyName(false));
                } catch (IOException ioe) {
                    ioe.printStackTrace();
                }
            }

            public void inquiryCompleted(int discType) {
                System.out.println("Device Inquiry completed!");
                synchronized (DefaultDeviceDetector.this) {
                    DefaultDeviceDetector.this.notifyAll();
                    fireDeviceDetectionComplete();
                }
            }

            public void serviceSearchCompleted(int transID, int respCode) {
            }

            public void servicesDiscovered(int transID,
                    ServiceRecord[] servRecord) {
            }
        };

        started = LocalDevice.getLocalDevice().getDiscoveryAgent().startInquiry(DiscoveryAgent.GIAC, listener);
    }

    public synchronized void waitForCompletion() throws InterruptedException {
        if (started) {
            wait();
        }
    }
}
