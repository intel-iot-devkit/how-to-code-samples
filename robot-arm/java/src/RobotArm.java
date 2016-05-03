import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.Timer;
import java.util.TimerTask;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import upm_joystick12.Joystick12;
import upm_uln200xa.ULN200XA;
import upm_uln200xa.ULN200XA.ULN200XA_DIRECTION_T;

public class RobotArm {

	private static ULN200XA stepperMotor1;
	private static ULN200XA stepperMotor2;
	private static Joystick12 joystick;
	public static final int CLOCKWISE = 1;
	public static final int COUNTER_CLOCKWISE = 2;

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		initiateSensors();
		moveMotorAccordingToJoysticks();
		setupServer();
	}

	/**
	 * periodically check for joystick input and move motors accordingly
	 */
	private static void moveMotorAccordingToJoysticks() {
		// TODO Auto-generated method stub
		Timer timer = new Timer();
		timer.schedule(new TimerTask() {

			@Override
			public void run() {
				// TODO Auto-generated method stub
				double x = scaleJoystickDirectionToMotorDirection(joystick.getXInput());
				double y = scaleJoystickDirectionToMotorDirection(joystick.getYInput());

				if (x == 1) { moveStepperMotor(stepperMotor1, CLOCKWISE); }
				if (x == -1) { moveStepperMotor(stepperMotor1, COUNTER_CLOCKWISE); }

				if (y == 1) { moveStepperMotor(stepperMotor2, CLOCKWISE); }
				if (y == -1) { moveStepperMotor(stepperMotor2, COUNTER_CLOCKWISE); }
			}
		}, 0,50);

	}

	/**
	 * initiate the sensors
	 */
	private static void initiateSensors() {
		// TODO Auto-generated method stub
		stepperMotor1 = new ULN200XA(4096, 9, 10, 11, 12);
		stepperMotor2 = new ULN200XA(4096, 4, 5, 6, 7);
		joystick = new Joystick12(0, 1);
	}

	/**
	 * Moves one of the stepper motors for a predetermined distance in a certain direction
	 */
	private static void moveStepperMotor(ULN200XA stepperMotor, int direction) {
		// TODO Auto-generated method stub
		stepperMotor.setSpeed(5);
		ULN200XA_DIRECTION_T motorDirection = upm_uln200xa.ULN200XA.ULN200XA_DIRECTION_T.DIR_CW;
		if(direction == COUNTER_CLOCKWISE){
			motorDirection = upm_uln200xa.ULN200XA.ULN200XA_DIRECTION_T.DIR_CCW;
		}
		stepperMotor.setDirection(motorDirection);
		stepperMotor.stepperSteps(1020);
	}

	/**
	 * translate the move of the joystick to the move of the motor
	 * @param joystickDirection - CLOCKWISE or COUNTER_CLOCKWISE
	 * @return number between -1 and 1 to determine movement of motor
	 */
	private static double scaleJoystickDirectionToMotorDirection(float joystickDirection){
		// convert down to 0..1
		double val = (joystickDirection + 0.5) / -0.4;
		if (val > 1) { val = 1; }
		if (val < 0) { val = 0; }

		// and then to -1..1 and round
		return Math.round(val * 2 - 1);
	}

	/**
	 * setup server endpoints and run http server
	 */
	private static void setupServer() {
		// TODO Auto-generated method stub
		ServerSetup server = new ServerSetup();
		server.setupServer(8080);
		server.addServletWithGetCall("/", new ServerSetup.GetHttpCall() {

			@Override
			public void runCall(HttpServletRequest request,
					HttpServletResponse response) throws IOException {
				response.getWriter().println( getIndexPage());
			}
		});
		server.addServletWithPostCall("/one-cw", new ServerSetup.PostHttpCall() {

			@Override
			public void runCall(HttpServletRequest request, HttpServletResponse response)
					throws IOException {
				moveStepperMotor(stepperMotor1, CLOCKWISE);
				response.getWriter().println("done");
			}
		});
		server.addServletWithPostCall("/one-ccw", new ServerSetup.PostHttpCall() {

			@Override
			public void runCall(HttpServletRequest request, HttpServletResponse response)
					throws IOException {
				moveStepperMotor(stepperMotor1, COUNTER_CLOCKWISE);
				response.getWriter().println("done");
			}
		});
		server.addServletWithPostCall("/two-cw", new ServerSetup.PostHttpCall() {

			@Override
			public void runCall(HttpServletRequest request, HttpServletResponse response)
					throws IOException {
				moveStepperMotor(stepperMotor2, CLOCKWISE);
				response.getWriter().println("done");

			}
		});
		server.addServletWithPostCall("/two-ccw", new ServerSetup.PostHttpCall() {

			@Override
			public void runCall(HttpServletRequest request, HttpServletResponse response)
					throws IOException {
				moveStepperMotor(stepperMotor2, COUNTER_CLOCKWISE);
				response.getWriter().println("done");
			}
		});
		server.run();

	}

	/**
	 * read index page from file and return it as string
	 * @return
	 */
	private static String getIndexPage() {
		// TODO Auto-generated method stub
		String sCurrentLine;
		BufferedReader indexFile;
		StringBuilder stringBuilder = null;
		try {
			indexFile = new BufferedReader(new FileReader("/var/RobotArm/index.html"));

			stringBuilder = new StringBuilder();
			while ((sCurrentLine = indexFile.readLine()) != null) {
				stringBuilder.append(sCurrentLine).append("\n");
			}
			indexFile.close();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			System.out.println("problem reading index.html from file");
			e.printStackTrace();
		}
		return stringBuilder.toString();
	}

}