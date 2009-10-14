package carrierpigeon.mobile;

import javax.bluetooth.BluetoothStateException;
import javax.bluetooth.DeviceClass;
import javax.bluetooth.DiscoveryAgent;
import javax.bluetooth.DiscoveryListener;
import javax.bluetooth.LocalDevice;
import javax.bluetooth.RemoteDevice;
import javax.bluetooth.ServiceRecord;
import javax.microedition.lcdui.Alert;

/**
 * A device discovery service that looks for bluetooth devices in the vicinity.
 * It runs in the background in a separate thread. 
 */
public class DefaultDeviceDetector extends DeviceDetector {

    private boolean started = false;

    private boolean completed = false;

    public synchronized void startDiscovery() throws BluetoothStateException {
        DiscoveryListener listener = new DiscoveryListener() {

            public void deviceDiscovered(RemoteDevice btDevice, DeviceClass cod) {
                devices.addElement(btDevice);
            }

            public void inquiryCompleted(int discType) {
                synchronized (DefaultDeviceDetector.this) {
                    fireDeviceDetectionComplete();
                    completed = true;
                    DefaultDeviceDetector.this.notifyAll();
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
        if (started && !completed) {
            wait();
        }
    }

    public synchronized boolean hasCompleted() {
        return completed;
    }
}
