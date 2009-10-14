package carrierpigeon.mobile;

import java.util.Vector;
import javax.bluetooth.BluetoothStateException;

/**
 * Abstract base class for bluetooth device discovery services. Interested
 * objects can listen to the detection state by registering a
 * {@link CompletionListener} with a device detector. Usually, it is not
 * possible to start two searches in parallel, since the detection blocks
 * the (hardware) bluetooth device.
 *
 * @see CompletionListener
 * @see DefaultDeviceDetector
 * @see MockDeviceDetector
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
     * Returns true if a search for bluetooth devices has completed,
     * false otherwise.
     */
    public abstract boolean hasCompleted();

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
