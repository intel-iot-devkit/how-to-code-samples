import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.Timer;
import java.util.TimerTask;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.google.gson.JsonArray;

public class RangeFinderScanner {

	private static boolean[] stateOfEachDegreeInRadious; 
	private static upm_rfr359f.RFR359F distanceInterruptor ;
	private static upm_uln200xa.ULN200XA stepperMotor;
	private static int currentDegree;


	public static void main(String[] args) {
		// TODO Auto-generated method stub
		stateOfEachDegreeInRadious = new boolean[360];
		for(int i=0; i<360; i++){
			stateOfEachDegreeInRadious[i] = false;
		}
		initiateSensors();
		setupServer();
		startSweepingStepperMotor();

	}

	/**
	 * Sweeps the stepper motor, to be able to get the range sensor
	 * reading for all 360 degress around the robot
	 */
	private static void startSweepingStepperMotor() {
		// TODO Auto-generated method stub
		currentDegree = 0;
		Timer timer = new Timer();
		timer.schedule(new TimerTask() {

			@Override
			public void run() {
				// TODO Auto-generated method stub
				boolean isObjectDetectedInCurrentDegree = distanceInterruptor.objectDetected();
				stateOfEachDegreeInRadious[currentDegree] = isObjectDetectedInCurrentDegree;
				System.out.println("degree : " + currentDegree + " state: " + isObjectDetectedInCurrentDegree);
				if(currentDegree == 359){
					currentDegree = 0;
				}
				else{
					currentDegree++;
					moveStepperMotor();
				}

			}
		}, 0,10);

	}

	/**
	 * Moves the stepper motor for a predetermined distance
	 */
	private static void moveStepperMotor() {
		// TODO Auto-generated method stub
		stepperMotor.setSpeed(5);
		stepperMotor.setDirection(upm_uln200xa.ULN200XA.ULN200XA_DIRECTION_T.DIR_CW);
		stepperMotor.stepperSteps(11);
	}

	/**
	 * initiate the sensors
	 */
	private static void initiateSensors() {
		// TODO Auto-generated method stub
		distanceInterruptor = new upm_rfr359f.RFR359F(2);
		stepperMotor = new upm_uln200xa.ULN200XA(4096, 9, 10, 11, 12);
	}


	/**
	 * setup server endpoints and run http server
	 */
	private static void setupServer() {
		// TODO Auto-generated method stub
		ServerSetup server = new ServerSetup();
		server.setupServer(8080);
		server.addServlet("/", new ServerSetup.GetCall() {

			@Override
			public void runCall(HttpServletRequest request,
					HttpServletResponse response) throws IOException {
				response.getWriter().println( getIndexPage());
			}
		});
		server.addServlet("/data.json", new ServerSetup.GetCall() {

			@Override
			public void runCall(HttpServletRequest request, HttpServletResponse response)
					throws IOException {
				response.setContentType("application/json");
				response.setStatus(HttpServletResponse.SC_OK);
				JsonArray responseJson = new JsonArray();
				for(int i=0; i < 360; i++){
					responseJson.add(stateOfEachDegreeInRadious[i]);
				}
				response.getWriter().println(responseJson.toString());
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
			indexFile = new BufferedReader(new FileReader("/var/RangeFinderScanner/index.html"));

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