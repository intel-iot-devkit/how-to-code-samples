import java.io.IOException;
import java.util.Properties;
import java.util.Timer;
import java.util.TimerTask;

/**
 * @author rhassidi
 *
 */
public class EarthquakeDetector {

    private static Properties config = new Properties();
    private static AccelerometerSensor accelerometer;
    private static LcdSensor lcd;
    private static float[] axis;
    private static boolean prev;
    private static boolean quake; 
    static int i = 0;

    /**
     * Initializing 3-axis and lcd sensors
     */
    private static void initSensors(){
	accelerometer = new AccelerometerSensor(2);
	lcd = new LcdSensor(6, config);
    }


    /**
     * loads the config file
     */
    private static void loadConfig(){
	try {
	    config.load(EarthquakeDetector.class.getClassLoader().getResourceAsStream("config.properties"));
	} catch (IOException ioe) {
	    ioe.printStackTrace();
	}
    }


    /**
     * Stops the verification message that is displayed on the lcd screen
     */
    private static void stopVerificationMeesage() {
	Timer stopVerificationTimer = new Timer();
	stopVerificationTimer.schedule(new TimerTask(){
	    public void run(){
		lcd.writeMessage("stopping");
	    }
	}, 15000);
    }


    /**
     * Verifies with the USGS API if an earthquake actually took place
     */
    private static void verify(){
	lcd.writeMessage("checking");
	String message="";
	try {
	    message = Utils.getRealEarthquakeStatus(config);
	} catch (IOException e) {
	    e.printStackTrace();

	}
	//message = "Earthquake!!!";
	//message = "No quake";

	if(!message.equals("")){
	    lcd.writeMessage(message);
	}

	stopVerificationMeesage();

    }


    /**
     * Every 100ms takes a sample from the 3-axis sensor to check if it detected an
     * earthquake
     */
    public static void checkQuake() {
	Timer alertTimer = new Timer();
	alertTimer.schedule(new TimerTask() {
	    public void run() {
		axis = accelerometer.getAccelerometer().getAcceleration();
		System.out.println("axis: {"+axis[0]+","+axis[1]+","+axis[2]+"}");
		quake = (axis[0] > 1);
		//if(i%10 == 0){
		//	quake = true;
		//}
		//i++;
		if(quake && !prev){
		    verify();
		}
		prev = quake;
//		quake = false;
	    }

	}, 100, 100);

    }

    /**
     *  Main function 
     *  calls checkQuake that checks every 100ms to see if there has been motion detected
     *  by the accelerometer. If so, it calls verify to check the USGS API and see if
     *  an earthquake has actually occurred, and displays info on the display
     */
    public static void main(String[] args) {
	prev = false;

	loadConfig();
	initSensors();
	checkQuake();


    }

}