import org.joda.time.DateTime;

/**
 * class holds information for a discovered BLE enabled device.
 * @author staite
 *
 */
public class DiscoveredDevice {
	
	private String deviceID;
	private short rssi;
	private DateTime lastSeen;
	
	public DiscoveredDevice(String deviceID, short rssi) {
		this.deviceID = deviceID;
		this.rssi = rssi;
		this.lastSeen = new DateTime();
		
	}
	
	public String getDeviceID() {
		return deviceID;
	}
	
	public void setDeviceID(String deviceID) {
		this.deviceID = deviceID;
	}
	
	public short getRssi() {
		return rssi;
	}
	
	public void setRssi(short rssi) {
		this.rssi = rssi;
	}

	public DateTime getLastSeen() {
		return lastSeen;
	}

	public void setLastSeen(DateTime lastSeen) {
		this.lastSeen = lastSeen;
	}
	
}
