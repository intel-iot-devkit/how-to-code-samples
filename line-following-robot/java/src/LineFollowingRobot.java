


import java.io.IOException;
import java.util.Properties;

import org.joda.time.DateTime;

import upm_uln200xa.ULN200XA;
import upm_uln200xa.ULN200XA.ULN200XA_DIRECTION_T;

public class LineFollowingRobot {

	private static Properties config = new Properties();
	private static ULN200XA leftMotor;
	private static ULN200XA rightMotor;
	private static upm_grovelinefinder.GroveLineFinder lineFinder;
	private static final int CLOCKWISE = 1;
	private static final int COUNTER_CLOCKWISE = 2;

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		initiateSensors();
		loadConfigurationFile();
		searchForLine();
		
	}

	/**
	 * load configuration file 
	 */
	private static void loadConfigurationFile() {
		// TODO Auto-generated method stub
		try {
			// Load configuration data from `config.properties` file. Edit this file
			// to change to correct values for your configuration
			config.load(LineFollowingRobot.class.getClassLoader().getResourceAsStream("config.properties"));
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/**
	 * constantly searches for a line, when found moves the robot forward
	 * and sends time of detection to Azure server
	 */
	private static void searchForLine() {
		// TODO Auto-generated method stub
		while(true){
			int steps = 512;
			System.out.println("looking for line");
			while(!lineFinder.blackDetected()){
				moveStepperMotor(leftMotor, CLOCKWISE,steps);
				steps += 512;

				moveStepperMotor(rightMotor, CLOCKWISE, steps);
				steps += 512;
			}
			System.out.println("line found");
			moveForward();
			Utils.notifyAzure(new DateTime().toDateTimeISO().toString(), config);
		}

	}

	/**
	 * moves robot forward
	 */
	private static void moveForward() {
		System.out.println("moving forward");
		moveStepperMotor(leftMotor, CLOCKWISE,1024);
		moveStepperMotor(rightMotor, CLOCKWISE,1024);
	}

	/**
	 * initiate sensors
	 */
	private static void initiateSensors() {
		// TODO Auto-generated method stub
		leftMotor = new ULN200XA(4096, 9, 10, 11, 12);
		rightMotor = new ULN200XA(4096, 4, 5, 6, 7);
		lineFinder = new upm_grovelinefinder.GroveLineFinder(2);
	}

	/**
	 * Moves one of the stepper motors for a predetermined distance in a certain direction
	 */
	private static void moveStepperMotor(ULN200XA stepperMotor, int direction, int steps) {
		// TODO Auto-generated method stub
		stepperMotor.setSpeed(5);
		ULN200XA_DIRECTION_T motorDirection = upm_uln200xa.ULN200XA.ULN200XA_DIRECTION_T.DIR_CW;
		if(direction == COUNTER_CLOCKWISE){
			motorDirection = upm_uln200xa.ULN200XA.ULN200XA_DIRECTION_T.DIR_CCW;
		}
		stepperMotor.setDirection(motorDirection);
		stepperMotor.stepperSteps(steps);
	}

}