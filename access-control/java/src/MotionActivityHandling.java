import java.util.Timer;
import java.util.TimerTask;

import upm_biss0001.BISS0001;


/**
 * @author rhassidi
 *
 */
public class MotionActivityHandling {
    private upm_biss0001.BISS0001 motionSensor;
    private LcdSensor lcd;
    private boolean prev = false;
    private Timer motionAlertTimer, motionDetectTimer;

    private final short TIMEOUT = 30000;
    private boolean validated = false, expectingCode = false, alarmInProgress = false;

    /**
     * The constructor that initializes the LCD and the motion sensor
     */
    public MotionActivityHandling(){
	this.motionSensor = new BISS0001(4);
	this.lcd = new LcdSensor(6);
    }

    
    /**
     * A callback function that runs "lookForMotion()" after TIMEOUT
     * 
     * @return the TimerTask to run
     */
    private TimerTask lookForMotionCallback(){
	return new TimerTask(){
	    public void run(){
		lookForMotion();
	    }
	};
    }
    
    /**
     * A callback function that measures the motion every 100ms
     * 
     * @return the TimerTask to run
     */
    private TimerTask checkMovementCallback(){
	return new TimerTask() {
	    public void run() {
		boolean movement = motionSensor.value();

		if(!prev && movement && !alarmInProgress){
		    motionDetectTimer.cancel();
		    motionDetectTimer.purge();
		    motionDetectedAlert();

		}
		prev = movement;
	    }
	};
    }
    
    
    /**
     *  A callback function that runs "startAlarm()" after TIMEOUT
     * 
     * @return the TimerTask to run
     */
    private TimerTask startAlarmCallback(){
	return new TimerTask(){
	    public void run(){
		startAlarm();
	    }
	};
    }

    /**
     * A callback function that validates the input code every 100ms
     * 
     * @return the TimerTask to run
     */
    private TimerTask motionAlertCallback() {
	return new TimerTask() {
	    public void run() {
		if(expectingCode && validated){
		    System.out.println("validated entry");
		    validated = false;
		    expectingCode = false;

		    if(alarmInProgress){
			stopAlarm();
		    }
		    motionAlertTimer.cancel();
		    motionAlertTimer.purge();
		    waitingMessage();
		}
	    }
	};
    }
    
    /**
     * Checks if the motion sensor detected movement.
     * If so, stops monitoring the motion sensor and calls motionDetectedAlert()
     */
    public void lookForMotion(){

	System.out.println("looking for motion");

	lcd.writeMessage("READY");
	motionDetectTimer = new Timer();
	motionDetectTimer.schedule(checkMovementCallback(), 1000,1000);
    }

    
    
    /**
     * Presents the blue alert message on the lcd.
     * Calls startAlarm,
     * and ends the alarm when the user inserts the alarm code in the website.
     */
    private void motionDetectedAlert(){
	System.out.println("motion detected");

	lcd.writeMessage("Alert");
	

	expectingCode = true;

	Timer startAlarmTimer = new Timer();
	startAlarmTimer.schedule(startAlarmCallback(), TIMEOUT);

	motionAlertTimer = new Timer();
	motionAlertTimer.schedule(motionAlertCallback(), 100,100);
    }
    
   

    /**
     * Presents the white waiting message on the lcd screen,
     * and starts looking for motion after TIMEOUT sec
     */
    private void waitingMessage(){
	System.out.println("waiting");
	lcd. writeMessage("Waiting 30s");

	Timer lookForMotionTimer = new Timer();
	lookForMotionTimer.schedule(lookForMotionCallback(), TIMEOUT);
	
    }

    /**
     * Stops the alarm
     */
    private void stopAlarm(){
	System.out.println("alarm stopping");
	alarmInProgress = false;
    }

    /**
     * Starts the alarm
     */
    private void startAlarm(){
	System.out.println("alarm stating");
	alarmInProgress = true;

	lcd.writeMessage("ALARM");

    }

    /**
     * @return true if the alarm is set on, and false otherwise
     */
    public boolean isExpectingCode(){
	return expectingCode;
    }

    /**
     * Sets validated to true
     */
    public void setValidatedTrue(){
	validated = true;
    }

}
