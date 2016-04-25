import java.io.IOException;
import java.util.Date;
import java.util.Properties;
import java.util.Timer;
import java.util.TimerTask;

import upm_rfr359f.RFR359F;

public class CloseCallReporter {

	private static upm_ublox6.Ublox6 gps;
	private static upm_rfr359f.RFR359F distanceInterrupter;
	private static boolean wasObjectDetectedLastTime = false;
	private static String parsedGPSData ="gps not available yet";
	private static Properties config = new Properties();

	public static void main(String[] args) {
		// TODO Auto-generated method stub

		try{
			loadConfigurationFile();
			setupGPS();
			setupDistanceInterrupter();
			listenToGPSSync();

		}
		catch(Exception e){
			e.printStackTrace();
			System.out.println("problem");
		}
	}

	/**
	 * load configuration file 
	 */
	private static void loadConfigurationFile() {
		// TODO Auto-generated method stub
		try {
			// Load configuration data from `config.properties` file. Edit this file
			// to change to correct values for your configuration
			config.load(CloseCallReporter.class.getClassLoader().getResourceAsStream("config.properties"));

		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/**
	 * instantiate distance interrupter sensor
	 */
	private static void setupDistanceInterrupter() {
		// TODO Auto-generated method stub
		distanceInterrupter = new RFR359F(2);
	}

	/**
	 * instantiate GPS sensor
	 */
	private static void setupGPS(){
		// TODO Auto-generated method stub
		gps = new upm_ublox6.Ublox6(0);
		// make sure port is initialized properly. 9600 baud is the default.
		if (!gps.setupTty()) {
			throw new RuntimeException("Failed to setup tty port parameters");
		}
	}

	/**
	 * get raw data from GPS sensor
	 * @return String representing raw GPS data
	 * @throws Exception
	 */
	private static String getGPSRawData() throws Exception{
		StringBuilder gpsRawDataString = new StringBuilder("");
		byte[] nmeaBuffer = new byte[256];

		// we don't want the read to block in this example, so always
		// check to see if data is available first.
		if (gps.dataAvailable()) {

			int rv = gps.readData(nmeaBuffer);
			if (rv > 0){
				for (int i = 0; i < rv; i++){
					gpsRawDataString.append((char) nmeaBuffer[i]);
				}
			}
			if (rv < 0) { // some sort of read error occurred
				System.err.println("Port read error.");
				throw new Exception();
			}

		}
		else{
			throw new Exception();
		}
		return gpsRawDataString.toString();
	}

	/**
	 * parses raw gps data to get part relating to location
	 * @param gpsRawData String raw data retrieved from GPS sensor
	 * @return String representing data relating to location.
	 */
	private static String getNMEAParsedGPSData(String gpsRawData) {

		String[] allSentences = gpsRawData.split("\r\n");
		for(String sentenceString: allSentences) {
			if(sentenceString.contains("GPGGA"))
				return sentenceString;
		}
		return "";
	}

	/**
	 * creates task which listens to GPS sensor as well as  distance interrupter
	 * and notifies the Azure cloud when an object is detected
	 */
	private static void listenToGPSSync(){
		Timer timer = new Timer();
		timer.schedule(new TimerTask() {

			@Override
			public void run() {
				// TODO Auto-generated method stub
				String rawGPSData=null;
				try {
					rawGPSData = getGPSRawData();
				} catch (Exception e) {
					// 
				}
				try {
					if(rawGPSData != null){
						String newParsedGPSData = getNMEAParsedGPSData(rawGPSData);
						if(newParsedGPSData.length() != 0){
							parsedGPSData = getNMEAParsedGPSData(rawGPSData);
							System.out.println("parsed data: " + parsedGPSData);
						}
					}

				} catch (Exception e) {
					// TODO Auto-generated catch block
					System.out.println("problem parsing gps data with nmea library");
				}
				boolean objectDetected = distanceInterrupter.objectDetected();
				if(objectDetected & !wasObjectDetectedLastTime){
					System.out.println("object detected");
					Utils.notifyAzure(new Date().toString() + " " + parsedGPSData, config);
				}
				wasObjectDetectedLastTime = objectDetected;
			}
		},0, 100);




	}
}


