import java.io.IOException;
import java.util.Date;
import java.util.Properties;
import java.util.Timer;
import java.util.TimerTask;

public class HomeFallTracker {

	private static Accelerometer accelerometer;
	private static OLEDScreen oledScreen;
	private static float[] previousForceValues = null;
	private static Properties config = new Properties();

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		loadConfigurationFile();
		initializeSensors();
		monitorFalls();
	}

	private static void initializeSensors() {
		// TODO Auto-generated method stub
		accelerometer = new Accelerometer();
		//oledScreen = new OLEDScreen();
	}

	/**
	 * load configuration file 
	 */
	private static void loadConfigurationFile() {
		// TODO Auto-generated method stub
		try {
			// Load configuration data from `config.properties` file. Edit this file
			// to change to correct values for your configuration
			config.load(HomeFallTracker.class.getClassLoader().getResourceAsStream("config.properties"));

		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	private static void monitorFalls() {
		// TODO Auto-generated method stub
		Timer fallMonitor = new Timer();
		fallMonitor.schedule(new TimerTask() {

			@Override
			public void run() {
				// TODO Auto-generated method stub
				float[] currentForceValues = accelerometer.getCurrentForceValues();
				System.out.println("current force: x:" + currentForceValues[0] + " y:" + currentForceValues[1] + " z:" + currentForceValues[2] );
				boolean isFallDetected =checkFallDetected(previousForceValues, currentForceValues);
				if(isFallDetected){
					alertFallDetected();
				}
				previousForceValues = currentForceValues;

			}
		}, 0,10);
	}


	protected static boolean checkFallDetected(float[] previousForceValues2, float[] currentForceValues) {
		// TODO Auto-generated method stub
		if(previousForceValues2 == null){
			return false;
		}
		else return (previousForceValues2[0] - currentForceValues[0] >=1  //check x axis
				|| previousForceValues2[1] - currentForceValues[1] >=1  //check y axis
				|| previousForceValues2[1] - currentForceValues[1] >=1 ); //check z axis
	}


	protected static void alertFallDetected() {
		// TODO Auto-generated method stub
		System.out.println("Fall Detected!");
		//oledScreen.displayMessageOnOLED("Fall Detected!");
		Utils.sendMessageWithTwilio("fall detected", config);
		Utils.notifyAzure(new Date().toString(), config);

	}

}