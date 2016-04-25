import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Properties;
import java.util.Set;
import java.util.Timer;
import java.util.TimerTask;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.joda.time.DateTime;

import com.google.gson.Gson;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;

public class WateringSystem {

	private static final String PUMP_ON = "on";
	private static final String PUMP_OFF = "off";
	private static MoistureController moistureController;
	private static FlowSensor flowSensor;

	private static WaterPump pump ;
	private static HashMap<Integer,Boolean> schedule;
	private static Properties config = new Properties();
	private static boolean shouldWaterPumpBeCurrentlyOn = false;


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
				response.getWriter().println("ok");
				response.setStatus(200);
			}
		});

		server.addServlet("/on", new ServerSetup.ServerCall() {

			@Override
			public void runCall(HttpServletRequest request, HttpServletResponse response)
					throws IOException {
				// TODO Auto-generated method stub
				changeCurrentHourStatusForPumpToOn();
				response.getWriter().println("");
			}
		},null);

		server.addServlet("/off", new ServerSetup.ServerCall() {

			@Override
			public void runCall(HttpServletRequest request, HttpServletResponse response)
					throws IOException {
				// TODO Auto-generated method stub
				changeCurrentHourStatusForPumpToOff();
				response.getWriter().println("");
			}
		},null);

		server.run();
	}


	/**
	 * initiated by the user, for the current hour the the pump should be running
	 */
	private static void changeCurrentHourStatusForPumpToOn(){

		DateTime currentTime = new DateTime();
		schedule.put(currentTime.getHourOfDay(), true);
		shouldWaterPumpBeCurrentlyOn = true;
		Utils.notifyAzure("on " + new DateTime().toDateTimeISO().toString(), config);
		pump.turnOn();
	}

	/**
	 * initiated by the user, for the current hour the the pump should not be running
	 */
	private static void changeCurrentHourStatusForPumpToOff(){

		DateTime currentTime = new DateTime();
		schedule.put(currentTime.getHourOfDay(), false);
		shouldWaterPumpBeCurrentlyOn = false;
		Utils.notifyAzure("off " + new DateTime().toDateTimeISO().toString(), config);
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
			indexFile = new BufferedReader(new FileReader("/var/WateringSystem/index.html"));

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
		flowSensor = new FlowSensor();
		pump = new WaterPump();
		moistureController = new MoistureController(1);
		moistureController.getCurrentMoistureValue();

	}

	/**
	 * get schedule for pump from user input on web interface and keep it in memory
	 * @param data String json containing setting for each hour of the day
	 */
	private static void receiveNewSchedule(String data){
		System.out.println("this is it\n" + data);
		Gson gson = new Gson();
		JsonObject jobj = gson.fromJson(data, JsonObject.class);
		Set<Entry<String,JsonElement>> valuePerHour = jobj.entrySet();
		for(Entry<String,JsonElement> valueForSpecificHour : valuePerHour){
			Boolean shouldPumpBeOn = valueForSpecificHour.getValue().getAsJsonObject().get(PUMP_ON).getAsBoolean();
			int hour = Integer.parseInt(valueForSpecificHour.getKey());
			schedule.put(hour, shouldPumpBeOn);
		}
	}

	/**
	 * sets schedule for pump to off throughout the whole day as default setting
	 */
	private static void initiateSchedule(){
		schedule = new HashMap<>();
		for(int i = 0; i < 24; i++){
			schedule.put(i, false);
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
				Utils.notifyAzure("moisture (" +Integer.toString(moistureValue) + ")", config);

			}
		}, 0, 30*1000);


	}

	/**
	 * initiate monitoring of Flow sensor - sampling it every 30 secs and comparing to schedule
	 */
	private static void initiateFlowChecks() {
		// TODO Auto-generated method stub
		TimerTask checkForFlowStatusTask = new TimerTask() {

			@Override
			public void run() {
				// TODO Auto-generated method stub
				boolean previousPumpActivityStatus = shouldWaterPumpBeCurrentlyOn;
				shouldWaterPumpBeCurrentlyOn = schedule.get(new DateTime().getHourOfDay());
				flowSensor.alertIfFlowStateDoesntMatchSetting(shouldWaterPumpBeCurrentlyOn, config);
				if(shouldWaterPumpBeCurrentlyOn != previousPumpActivityStatus){
					if(shouldWaterPumpBeCurrentlyOn){
						pump.turnOn();
					}
					else{
						pump.turnOff();; 
					}
				}
			}
		};
		Timer timer = new Timer();
		timer.schedule(checkForFlowStatusTask, 0,1000);
	}



	/**
	 * create json representation of pump schedule
	 * @return String json representation of pump schedule
	 */
	private static String createScheduleInJsonFormat(){
		JsonObject httpJson = new JsonObject();
		JsonObject scheduleInJson = new JsonObject();
		for(Map.Entry<Integer,Boolean> hourPumpStatus : schedule.entrySet()){

			JsonObject stateForHourObj = new JsonObject();
			stateForHourObj.addProperty(PUMP_ON, hourPumpStatus.getValue());
			stateForHourObj.addProperty(PUMP_OFF, !hourPumpStatus.getValue());

			scheduleInJson.add(hourPumpStatus.getKey().toString(),stateForHourObj);
		}
		System.out.println("sending json :\n" + scheduleInJson.toString());
		httpJson.add("data", scheduleInJson);
		return httpJson.toString();

	}


	/**
	 * load configuration file 
	 */
	private static void loadConfigurationFile() {
		// TODO Auto-generated method stub
		try {
			// Load configuration data from `config.properties` file. Edit this file
			// to change to correct values for your configuration
			config.load(WateringSystem.class.getClassLoader().getResourceAsStream("config.properties"));

		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public static void main(String[] args) {
		loadConfigurationFile();
		initiateSensors();
		initiateSchedule();
		initiateFlowChecks();
		initiateMoistureMonitor();
		setupServer();
	}


}