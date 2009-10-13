package carrierpigeon;

import javax.bluetooth.BluetoothStateException;
import javax.bluetooth.RemoteDevice;

/**
 *
 * @author julius
 */
public class MockDeviceDetector extends DeviceDetector {

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
                        MockDeviceDetector.this.notifyAll();
                        fireDeviceDetectionComplete();
                    }
                }
            }
        }.start();
    }

    public void waitForCompletion() throws InterruptedException {
        synchronized (this) {
            wait();
        }
    }
}
