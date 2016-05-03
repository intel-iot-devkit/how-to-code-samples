import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.Properties;
import java.util.Timer;
import java.util.TimerTask;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.google.gson.JsonObject;

import upm_grovespeaker.GroveSpeaker;
import upm_yg1006.YG1006;

public class SmartStoveTop {

	private static float targetTemperature;
	private static upm_otp538u.OTP538U temperatureSensor;
	private static upm_yg1006.YG1006 flameSensor;
	private static upm_grovespeaker.GroveSpeaker speaker;
	private static Properties config = new Properties();
	
	private static float previousTemperature;
	private static boolean previousPresenceOfFire;

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		loadConfigurationFile();
		setupServer();
		initiateSensors();
		monitorHighTemperatureAndFire();
		logTemperatureToAzure();
	}


	/**
	 * initiate all sensor objects
	 */
	private static void initiateSensors(){
		temperatureSensor = new upm_otp538u.OTP538U(0, 1);
		flameSensor = new YG1006(4);
		speaker = new GroveSpeaker(5);
	}

	/**
	 * check for fire and for temperature higher than threshold and start 
	 * alarms accordingly
	 */
	private static void monitorHighTemperatureAndFire() {
		// TODO Auto-generated method stub
		 previousTemperature = 0;
		 previousPresenceOfFire = false;

		Timer monitoringTimer = new Timer();
		monitoringTimer.schedule(new TimerTask() {

			@Override
			public void run() {
				// TODO Auto-generated method stub
				float temperature = temperatureSensor.objectTemperature();
				boolean isFireDetected = flameSensor.flameDetected();
				System.out.println("current temperature: " + temperature);
				System.out.println("is fire detected: " + isFireDetected);
				
				if(previousTemperature <= targetTemperature && temperature > targetTemperature){
					startTemperatureAlarm();
				}
				if(!previousPresenceOfFire && isFireDetected){
					startFireAlarm();
				}
				previousTemperature = temperature;
				previousPresenceOfFire = isFireDetected;
				
				
			}
		},0, 1000);

	}

	/**
	 * sound fire alarm through speaker
	 */
	private static void startFireAlarm() {
		// TODO Auto-generated method stub
		System.out.println("fire detected ");
		int i = 0;
		  while (i < 10) {
		    speaker.playSound('a', true, "high");
		    speaker.playSound('c', true, "high");
		    speaker.playSound('g', true, "med");
		    i++;
		  }
	}


	/**
	 * sound high temperature alarm through speaker
	 */
	private static void startTemperatureAlarm() {
		// TODO Auto-generated method stub
		System.out.println("high temperature detected");
		speaker.playSound('a', true, "low");
		speaker.playSound('c', true, "low");
		speaker.playSound('d', true, "low");
		speaker.playSound('b', false, "low");
	}


	/**
	 * send current temperature data to azure server
	 */
	private static void logTemperatureToAzure(){
		Timer logTemperatureToAzureTimer = new Timer();
		logTemperatureToAzureTimer.schedule(new TimerTask() {

			@Override
			public void run() {
				// TODO Auto-generated method stub
				Utils.notifyAzure(temperatureSensor.objectTemperature() + " degrees", config);
			}
		}, 0,60*1000);
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
				String newTargetTemperature = request.getParameter("temp");
				if(newTargetTemperature != null){
					targetTemperature = Float.parseFloat(newTargetTemperature);
				}
				response.getWriter().println( getIndexPage());
			}
		});
		server.addServlet("/temp.json", new ServerSetup.GetCall() {

			@Override
			public void runCall(HttpServletRequest request, HttpServletResponse response)
					throws IOException {
				response.setContentType("application/json");
			    response.setStatus(HttpServletResponse.SC_OK);
			    JsonObject responseJson = new JsonObject();
			    responseJson.addProperty("temp", targetTemperature);
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
			indexFile = new BufferedReader(new FileReader("/var/SmartStoveTop/index.html"));

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

	/**
	 * load configuration file and retrieve threshold for temperature alert
	 */
	private static void loadConfigurationFile() {
		// TODO Auto-generated method stub
		try {
			// Load configuration data from `config.properties` file. Edit this file
			// to change to correct values for your configuration
			config.load(SmartStoveTop.class.getClassLoader().getResourceAsStream("config.properties"));
			targetTemperature = Float.parseFloat(config.getProperty("TEMPERATURE_THRESHOLD"));
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

}