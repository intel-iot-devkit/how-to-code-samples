
import java.io.IOException;

import java.util.Properties;
import java.util.Timer;
import java.util.TimerTask;

import upm_gas.TP401;
import upm_grovespeaker.GroveSpeaker;


/**
 * continuously checks the air quality for airborne contaminates;
 * sounds an audible warning when the air quality is unhealthy;
 * stores a record of each time the air quality sensor detects contaminates,
 * using cloud-based data storage.
 * 
 * @author rhassidi
 *
 */
public class AirQualitySensor {


    private static Properties config = new Properties();
    private static upm_grovespeaker.GroveSpeaker speaker;
    private static upm_gas.TP401 airQualitySensor;
    private static int prev = 0;

    /**
     * Initializes sensors
     */
    private static void initSensors(){
	speaker = new GroveSpeaker(5);
	airQualitySensor = new TP401(0);
    }

    /**
     * Plays a chime sound using the Grove speaker
     */
    public static void chime(){
	speaker.playSound('a', true, "low");
	speaker.playSound('c', true, "low");
	speaker.playSound('g', true, "low");
    }

    
    /**
     * Alert user that the air quality level has exceeded
     * the allowed threshold of safety
     */
    public static void alertBadAirQuality(){
	Utils.NotifyAzure(config);
	chime();
    }

    /**
     * Checks every 1sec if the air quality is higher than the threshold that is 
     * defined in the config file.
     */
    private static void checkAirQuality() {
	int threshold = Integer.parseInt(config.getProperty("THRESHOLD"));

	Timer alertTimer = new Timer();
	alertTimer.schedule(new TimerTask() {
	    public void run() {
		int quality = airQualitySensor.getSample();
		System.out.println("quality: " + quality);
		if(prev <= threshold && quality > threshold && prev != 0){
		    alertBadAirQuality();
		}
		prev = quality; 
	    }

	}, 1000, 1000);
    }
    
    /**
     * Main function checks the air quality every 1 second,
     * then calls the `alert()` function if quality level has exceeded
     * the allowed threshold of safety
     */
    public static void main(String[] args) {
	System.out.println("Starting main");
	try {
	    config.load(AirQualitySensor.class.getClassLoader().getResourceAsStream("config.properties"));
	} catch (IOException e) {
	    e.printStackTrace();
	}
	initSensors();
	checkAirQuality();
    }
}