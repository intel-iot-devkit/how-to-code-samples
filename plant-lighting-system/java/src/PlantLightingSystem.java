import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map.Entry;
import java.util.Map;
import java.util.Properties;
import java.util.Set;
import java.util.Timer;
import java.util.TimerTask;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.joda.time.DateTime;

import mraa.Platform;
import mraa.mraa;

import com.google.gson.Gson;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;

public class PlantLightingSystem {

	private static final String LIGHTS_ON = "on";
	private static final String LIGHTS_OFF = "off";

	private static LCDController lcdController;
	private static LightController lightController;
	private static MoistureController moistureController;
	private static HashMap<Integer,Boolean> schedule;
	private static Properties config = new Properties();
	private static boolean shouldLightBeCurrentlyOn = false;

	static {
		try {
			System.loadLibrary("mraajava");
		} catch (UnsatisfiedLinkError e) {
			System.err.println(
					"Native code library failed to load. See the chapter on Dynamic Linking Problems in the SWIG Java documentation for help.\n" +
							e);
			System.exit(1);
		}
	}

	/**
	 * defines web server endpoints and defines a method to run for each
	 */
	private static void setupServer(){
		ServerSetup server = new ServerSetup();
		server.setupServer(8080);
		server.addServlet("/",
				new ServerSetup.ServerCall() {

			@Override
			public void runCall(HttpServletRequest request, HttpServletResponse response)
					throws IOException {
				// TODO Auto-generated method stub

				response.getWriter().println(getIndexPageWithMoistureLevels());
			}
		},null);

		server.addServlet("/schedule",
				new ServerSetup.ServerCall() {

			@Override
			public void runCall(HttpServletRequest request, HttpServletResponse response)
					throws IOException {
				// TODO Auto-generated method stub
				response.addHeader("Content-type", "application/json");
				response.getWriter().write(createScheduleInJsonFormat());
			}
		},new ServerSetup.ServerCall() {

			@Override
			public void runCall(HttpServletRequest request, HttpServletResponse response)
					throws IOException {
				// TODO Auto-generated method stub
				StringBuilder buffer = new StringBuilder();
				BufferedReader reader = request.getReader();
				String line;
				while ((line = reader.readLine()) != null) {
					buffer.append(line);
				}
				String data = buffer.toString();
				reader.close();
				receiveNewSchedule(data);
				response.setStatus(200);
			}
		});

		server.addServlet("/on", new ServerSetup.ServerCall() {

			@Override
			public void runCall(HttpServletRequest request, HttpServletResponse response)
					throws IOException {
				// TODO Auto-generated method stub
				changeCurrentHourStatusToLightOn();
			}
		},null);

		server.addServlet("/off", new ServerSetup.ServerCall() {

			@Override
			public void runCall(HttpServletRequest request, HttpServletResponse response)
					throws IOException {
				// TODO Auto-generated method stub
				changeCurrentHourStatusToLightOff();
			}
		},null);

		server.run();
	}

	
	/**
	 * initiated by the user, for the current hour the light sensor should look for light
	 */
	private static void changeCurrentHourStatusToLightOn(){

		DateTime currentTime = new DateTime();
		schedule.put(currentTime.getHourOfDay(), true);
		shouldLightBeCurrentlyOn = true;
		lcdController.displayMessageOnLcd("on", 0);
		Utils.notifyAzure("on", config);
	}
	
	/**
	 * initiated by the user, for the current hour the light sensor should look for no light
	 */
	private static void changeCurrentHourStatusToLightOff(){
		
		DateTime currentTime = new DateTime();
		schedule.put(currentTime.getHourOfDay(), false);
		shouldLightBeCurrentlyOn = false;
		lcdController.displayMessageOnLcd("off", 0);
		Utils.notifyAzure("off", config);
	}

	/**
	 * read index page from file and serve it
	 * @return String index page
	 */
	private static String getIndexPageWithMoistureLevels(){
		String sCurrentLine;
		BufferedReader indexFile;
		StringBuilder stringBuilder = null;
		try {
			indexFile = new BufferedReader(new FileReader("/var/PlantLightingSystem/index.html"));

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
		return stringBuilder.toString().replace("$MOISTUREDATA$", moistureController.getHTMLForAllLoggedMoistureValues());
	}


	/**
	 * initiate all sensors
	 */
	private static void initiateSensors() {
		// TODO Auto-generated method stub
		lcdController = new LCDController(0);
		lightController = new LightController(0);
		moistureController = new MoistureController(1);
		moistureController.getCurrentMoistureValue();

	}

	/**
	 * get schedule for light from user input on web interface and keep it in memory
	 * @param data String json containing setting for each hour of the day
	 */
	private static void receiveNewSchedule(String data){
		System.out.println("this is it\n" + data);
		Gson gson = new Gson();
		JsonObject jobj = gson.fromJson(data, JsonObject.class);
		Set<Entry<String,JsonElement>> valuePerHour = jobj.entrySet();
		for(Entry<String,JsonElement> valueForSpecificHour : valuePerHour){
			Boolean isOn = valueForSpecificHour.getValue().getAsJsonObject().get(LIGHTS_ON).getAsBoolean();
			int hour = Integer.parseInt(valueForSpecificHour.getKey());
			schedule.put(hour, isOn);
		}
	}
	
	/**
	 * sets schedule for lights off throughout the whole day
	 */
	private static void initiateSchedule(){
		schedule = new HashMap<>();
		for(int i = 0; i < 24; i++){
			schedule.put(i, true);
		}
	}

	/**
	 * initiate monitoring of moisture sensor - sampling it every 30 secs
	 */
	private static void initiateMoistureMonitor(){

		Timer moistureMonitorTimer = new Timer();
		moistureMonitorTimer.schedule(new TimerTask() {

			@Override
			public void run() {
				// TODO Auto-generated method stub
				int moistureValue = moistureController.getCurrentMoistureValue();
				lcdController.displayMessageOnLcd("moisture (" + moistureValue + ")", 0); 
				Utils.notifyAzure(Integer.toString(moistureValue), config);

			}
		}, 0, 30*1000);


	}

	/**
	 * initiate monitoring of light sensor - sampling it every 30 secs and comparing to schedule
	 */
	private static void initiateLightChecks() {
		// TODO Auto-generated method stub
		TimerTask checkForLightOnTask = new TimerTask() {

			@Override
			public void run() {
				// TODO Auto-generated method stub
				shouldLightBeCurrentlyOn = schedule.get(new DateTime().getHourOfDay());

				if((lightController.getCurrentValue() < 2 && shouldLightBeCurrentlyOn)
						|| lightController.getCurrentValue() > 4 && !shouldLightBeCurrentlyOn){
					System.out.println("alerting");
					lcdController.displayMessageOnLcd("Lighting alert",0);
					Utils.sendMessageWithTwilio("Lighting alert", config);
				}
			}
		};
		Timer timer = new Timer();
		timer.schedule(checkForLightOnTask, 0,1000);

	}

	/**
	 * create json representation of light schedule
	 * @return String json representation of light schedule
	 */
	private static String createScheduleInJsonFormat(){
		JsonObject httpJson = new JsonObject();
		JsonObject scheduleInJson = new JsonObject();
		for(Map.Entry<Integer,Boolean> hourLightStatus : schedule.entrySet()){

			JsonObject stateForHourObj = new JsonObject();
			stateForHourObj.addProperty(LIGHTS_ON, hourLightStatus.getValue());
			stateForHourObj.addProperty(LIGHTS_OFF, !hourLightStatus.getValue());

			scheduleInJson.add(hourLightStatus.getKey().toString(),stateForHourObj);
		}
		System.out.println("sending json :\n" + scheduleInJson.toString());
		httpJson.add("data", scheduleInJson);
		return httpJson.toString();

	}

	public static void main(String[] args) {

		Platform platform = mraa.getPlatformType();
		if (platform != Platform.INTEL_GALILEO_GEN1 &&
				platform != Platform.INTEL_GALILEO_GEN2 &&
				platform != Platform.INTEL_EDISON_FAB_C) {
			System.err.println("Unsupported platform, exiting");
			return;
		}
		try {
			// Load configuration data from `config.properties` file. Edit this file
			// to change to correct values for your configuration
			config.load(PlantLightingSystem.class.getClassLoader().getResourceAsStream("config.properties"));
		} catch (IOException e) {
			e.printStackTrace();
		}
		initiateSensors();
		initiateSchedule();
		initiateLightChecks();
		initiateMoistureMonitor();
		setupServer();
	}


}