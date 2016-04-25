import java.io.IOException;
import java.util.Properties;
import java.util.Timer;
import java.util.TimerTask;

import upm_grovemoisture.GroveMoisture;
import upm_grovespeaker.GroveSpeaker;


/**
 * 
 * @author rhassidi
 * 
 * tore water detection data using Azure Redis Cache* from Microsoft* Azure*, cloud 
 * services for connecting IoT solutions including data analysis, machine learning, 
 * and a variety of productivity tools to simplify the process of 
 * connecting your sensors to the cloud and getting your IoT project up and running quickly.
 *
 */
public class StorageUnitFloodDetector {

    private static Properties config = new Properties();
    private static upm_grovespeaker.GroveSpeaker speakerSensor;
    private static upm_grovemoisture.GroveMoisture moistureSensor;
    private static int prev = 0;

    /**
     * Initializes sensors
     */
    private static void initSensors(){
	speakerSensor = new GroveSpeaker(5);
	moistureSensor = new GroveMoisture(0);
    }

    /**
     * Plays a chime sound using the Grove speaker
     */
    public static void chime(){
	speakerSensor.playSound('a', true, "low");
	speakerSensor.playSound('c', true, "low");
	speakerSensor.playSound('g', true, "low");
    }

    /**
     * Alert user that the moisture level is high
     */
    public static void alertHighMoisture(){
	Utils.NotifyAzure(config);
	chime();
    }

    /**
     * Checks every 1sec if the moisture level is high and alerts about it accordingly
     */
    public static void checkStorageUnitFlood(){
	Timer alertTimer = new Timer();
	alertTimer.schedule(new TimerTask() {
	    public void run() {
		int MoistureValue = moistureSensor.value();
		System.out.println("MoistureValue = " + MoistureValue);
		if(prev == 0 && MoistureValue != 0){
		    alertHighMoisture();
		}
		prev = MoistureValue; 
	    }

	}, 1000, 1000);
    }

    /**
     * Loads the configuration file
     */
    private static void loadConfigFile() {
	try {
	    config.load(StorageUnitFloodDetector.class.getClassLoader().getResourceAsStream("config.properties"));
	} catch (IOException e) {
	    e.printStackTrace();
	}
    }
    
    /**
     * The main function monitors the connected hardware every 1
     * second to check if moisture has been detected,
     * indicating a possible flood.
     * If so, it calls the `alertHighMoisture()` function.
     */
    public static void main(String[] args) {
	System.out.println("Starting main");
	loadConfigFile();
	initSensors();
	checkStorageUnitFlood();
    }

   

}