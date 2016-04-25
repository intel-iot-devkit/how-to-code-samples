import java.io.IOException;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map.Entry;
import java.util.Properties;
import java.util.Timer;
import java.util.TimerTask;

import org.joda.time.DateTime;

import tinyb.BluetoothDevice;

public class BleScanBracelet {

	private static Properties config = new Properties();
	private static OLEDScreen OLEDScreen;
	private static HashMap<String,DiscoveredDevice> previouslyDiscoveredDevices = new HashMap<String,DiscoveredDevice>();
	private static final int RSSI_THRESHOLD = -1000;
	private static final int DEVICE_AVAILABLE_GRACE_PERIOD = 10000;

	public static void main(String[] args) {
		loadConfigurationFile();
		initiateSensors();
		DiscoverDevicesPeriodically();
		checkForNoLongerDetectedDevicesPeriodically();
	}

	/**
	 * initiate sensor objects
	 */
	private static void initiateSensors() {
		OLEDScreen = new OLEDScreen();
	}

	/**
	 * load configuration file 
	 */
	private static void loadConfigurationFile() {
		try {
			// Load configuration data from `config.properties` file. Edit this file
			// to change to correct values for your configuration
			config.load(BleScanBracelet.class.getClassLoader().getResourceAsStream("config.properties"));
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/**
	 * discover BLE enabled devices periodically and handle their discovery.
	 */
	private static void DiscoverDevicesPeriodically(){

		Timer timer = new Timer();
		timer.schedule(new TimerTask() {

			@Override
			public void run() {
				// TODO Auto-generated method stub
				System.out.println("discover");
				List<BluetoothDevice> bleDevices = null;
				tinyb.BluetoothManager.getBluetoothManager().startDiscovery();
				bleDevices = tinyb.BluetoothManager.getBluetoothManager().getDevices();
				for(BluetoothDevice device : bleDevices){
					HandleDeviceDiscovered(device);
				}
			}
		}, 0,3000);

	}

	/**
	 * check for BLE enabled devices which have not been seen  within the grace period.
	 */
	private static void checkForNoLongerDetectedDevicesPeriodically(){

		Timer timer = new Timer();
		timer.schedule(new TimerTask() {

			@Override
			public void run() {
				// TODO Auto-generated method stub

				synchronized (previouslyDiscoveredDevices) {
					System.out.println("check for not detected");
					DateTime currentTimeWithSubstractedGracePeriod = new DateTime();
					currentTimeWithSubstractedGracePeriod = currentTimeWithSubstractedGracePeriod.minusMillis(DEVICE_AVAILABLE_GRACE_PERIOD);
					Iterator<Entry<String, DiscoveredDevice>> previouslyDiscoveredDevicesIterator = previouslyDiscoveredDevices.entrySet().iterator();
					while (previouslyDiscoveredDevicesIterator.hasNext()){
						Entry<String,DiscoveredDevice> item = (Entry<String, DiscoveredDevice>) previouslyDiscoveredDevicesIterator.next();
						if(item.getValue().getLastSeen().isBefore(currentTimeWithSubstractedGracePeriod)) {
							handleDeviceExited(item.getKey(),previouslyDiscoveredDevicesIterator);
						}
					}
				}

			}
		}, 0,DEVICE_AVAILABLE_GRACE_PERIOD / 4);


	}

	/**
	 * when a device is missing this method takes care of notifying the Azure server, as well as
	 * the user through OLED screen
	 * @param deviceName name of BLE enabled device
	 * @param previouslyDiscoveredDevicesIterator iterator containing previously discovered devices,
	 * passed in order to prevent a ConcurrentModificationException exception
	 */
	private static void handleDeviceExited(String deviceName, Iterator<Entry<String, DiscoveredDevice>> previouslyDiscoveredDevicesIterator) {

		previouslyDiscoveredDevicesIterator.remove();
		String message = deviceName +" exited";
		Utils.notifyAzure(message, config);
		System.out.println(message);
		OLEDScreen.writeToScreen(message);
	}

	/**
	 * when a device is discovered this method takes care of notifying the Azure server, as well as
	 * the user through OLED screen if the device hasn't been seen in the last grace period and its
	 * RSSI is higher than threshold.
	 * if the device has recently been discovered the lastSeen property is set for the current time 
	 * and date.
	 * @param device BLE discovered device
	 */
	private static void HandleDeviceDiscovered(BluetoothDevice device) {
		synchronized (previouslyDiscoveredDevices) {
			String deviceName = device.getName();
			if (previouslyDiscoveredDevices.containsKey(deviceName)) {
				previouslyDiscoveredDevices.get(deviceName).setLastSeen(
						new DateTime());
				System.out.println("updating date for " + deviceName);
			} else if (device.getRssi() >= RSSI_THRESHOLD) {
				DiscoveredDevice discoveredDevice = new DiscoveredDevice(
						deviceName, device.getRssi());
				previouslyDiscoveredDevices.put(deviceName, discoveredDevice);
				String message = deviceName + " entered";
				Utils.notifyAzure(message, config);
				System.out.println(message);
				OLEDScreen.writeToScreen(message);
			}
		}



	}

}