package carrierpigeon;

import java.io.IOException;
import java.util.Vector;
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
public abstract class DeviceDetector {

    protected final Vector devices = new Vector();

    private final Vector listenerList = new Vector();

    public abstract void startDiscovery() throws BluetoothStateException;
    public abstract void waitForCompletion() throws InterruptedException;

    public Vector getDevices() {
        return devices;
    }

    public void addCompletionListener(CompletionListener l) {
        listenerList.addElement(l);
    }

    public void removeCompletionListener(CompletionListener l) {
        listenerList.removeElement(l);
    }

    protected void fireDeviceDetectionComplete() {
        for (int i = 0; i < listenerList.size(); i++) {
            CompletionListener l = (CompletionListener) listenerList.elementAt(i);
            l.deviceDetectionComplete();
        }
    }

    public interface CompletionListener {

        public void deviceDetectionComplete();
    }
}
