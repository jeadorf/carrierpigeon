package carrierpigeon.mobile;

import javax.bluetooth.BluetoothStateException;
import javax.bluetooth.RemoteDevice;

/**
 * A fake device detector that simulates a real device discovery mechanism. It
 * needs some time (in fact it sleeps) and adds some fake devices to its list
 * of detected devices. These devices have a name and a bluetooth address, but
 * attempts to connect with these devices will usually fail, unless there is
 * a bluetooth device with one of the fake addresses in the vicinity (very
 * unlikely).
 */
public class MockDeviceDetector extends DeviceDetector {

    private boolean completed = false;

    public void startDiscovery() throws BluetoothStateException {
        new Thread() {

            public void run() {
                System.out.println("FakeDeviceDetector running");
                try {
                    sleep(1000);
                    devices.addElement(new RemoteDevice("00126F037095") {

                        public String getFriendlyName(boolean alwaysAsk) {
                            return "eumel (fake)";
                        }
                    });
                    sleep(1000);
                    devices.addElement(new RemoteDevice("00126F030195") {

                        public String getFriendlyName(boolean alwaysAsk) {
                            return "fake1";
                        }
                    });
                    sleep(1000);
                    devices.addElement(new RemoteDevice("00126F030295") {

                        public String getFriendlyName(boolean alwaysAsk) {
                            return "fake2";
                        }
                    });
                } catch (InterruptedException ex) {
                    ex.printStackTrace();
                } finally {
                    synchronized (MockDeviceDetector.this) {
                        fireDeviceDetectionComplete();
                        completed = true;
                        MockDeviceDetector.this.notifyAll();
                    }
                }
            }
        }.start();
    }

    public void waitForCompletion() throws InterruptedException {
        if (!completed) {
            synchronized (this) {
                wait();
            }
        }
    }

    public synchronized boolean hasCompleted() {
        return completed;
    }
}
