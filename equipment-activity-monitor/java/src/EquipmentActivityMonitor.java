import java.io.IOException;
import java.util.Date;
import java.util.Properties;
import java.util.Timer;
import java.util.TimerTask;

import upm_ldt0028.LDT0028;
import upm_mic.Microphone;

public class EquipmentActivityMonitor {

	private static LcdScreen lcdScreen;
	private static upm_ldt0028.LDT0028 vibrationSensor;
	private static upm_mic.Microphone soundSensor;
	private static Properties config = new Properties();
	private static int noiseThreshold;
	private static int vibrationThreshold;
	private static boolean isNotificationInProgress = false;

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		loadConfigurationFile();
		initiateSensors();
		monitorEquipment();
	}


	/**
	 * load configuration file and retrieve vibration and noise thresholds
	 */
	private static void loadConfigurationFile() {
		// TODO Auto-generated method stub
		try {
			// Load configuration data from `config.properties` file. Edit this file
			// to change to correct values for your configuration
			config.load(EquipmentActivityMonitor.class.getClassLoader().getResourceAsStream("config.properties"));
			noiseThreshold =Integer.parseInt(config.getProperty("NOISE_THRESHOLD"));
			vibrationThreshold =Integer.parseInt(config.getProperty("VIBRATION_THRESHOLD"));
		} catch (IOException e) {
			e.printStackTrace();
		}
	}


	/**
	 * instantiate sensor helper objects
	 */
	private static void initiateSensors() {
		// TODO Auto-generated method stub
		lcdScreen = new LcdScreen();
		vibrationSensor = new LDT0028(0);
		soundSensor = new Microphone(0);
	}

	/**
	 * run periodic task which compares vibration and noise sensors 
	 * to thresholds and notifies accordingly
	 */
	private static void monitorEquipment() {
		// TODO Auto-generated method stub
		Timer equipmentCheckTimer = new Timer();
		equipmentCheckTimer.schedule(new TimerTask() {

			@Override
			public void run() {
				// TODO Auto-generated method stub
				boolean isVibration = (vibrationSensor.getSample() >= vibrationThreshold);
				boolean isNoise = false;

				upm_mic.thresholdContext ctx = new upm_mic.thresholdContext();
				ctx.setAverageReading(0);
				ctx.setRunningAverage(0);
				ctx.setAveragedOver(2);
				short[] buffer = new short[128];
				int len = soundSensor.getSampledWindow(2, buffer);
				if (len == 128) {
					int threshold = soundSensor.findThreshold(ctx, noiseThreshold, buffer);
					if (threshold != 0){
						isNoise = (ctx.getRunningAverage() >= noiseThreshold);
					}
				}

				if(isNoise && isVibration && !isNotificationInProgress){
					alertEquipmentMoved();
				}
				if(!(isNoise && isVibration) && isNotificationInProgress){
					stopAlert();
				}
				isNotificationInProgress = isNoise && isVibration;
			}

		},0,20);
	}

	/**
	 * alert equipment moved
	 */
	private static void alertEquipmentMoved() {
		// TODO Auto-generated method stub
		Utils.notifyAzure(("start " + new Date().toString()), config);
		System.out.println("equipment moved!");

		lcdScreen.displayMessageOnLcd("EQUIPMENT IN USE", 0);
		lcdScreen.setLcdColor("white");
	}

	/**
	 * stop alerting of equipment moving
	 */
	private static void stopAlert() {
		// TODO Auto-generated method stub
		Utils.notifyAzure(("stop " + new Date().toString()), config);
		System.out.println("equipment stopped moving!");
		lcdScreen.clearLCDScreen();
	}

}