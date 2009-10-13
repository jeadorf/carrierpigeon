package carrierpigeon;

import java.util.Vector;
import javax.bluetooth.BluetoothStateException;

/**
 *
 */
public abstract class DeviceDetector {

    protected final Vector devices = new Vector();

    private final Vector listenerList = new Vector();

    /**
     * Starts bluetooth device detection within a separate thread. This method
     * does not block. It returns immediately.
     * 
     * @throws BluetoothStateException
     */
    public abstract void startDiscovery() throws BluetoothStateException;

    /**
     * Waits until the detection process finishs.
     * 
     * @throws InterruptedException
     */
    public abstract void waitForCompletion() throws InterruptedException;

    /**
     * Gets all devices that have been discovered so far. Returned vector
     * contains instances of RemoteDevice.
     *
     * @return
     */
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
