import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.Properties;
import java.util.Timer;
import java.util.TimerTask;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.joda.time.DateTime;
import org.joda.time.Period;
import org.joda.time.format.DateTimeFormat;
import org.joda.time.format.DateTimeFormatter;

import upm_grove.GroveButton;
import upm_grove.GroveRotary;
import mraa.Platform;
import mraa.mraa;

import com.google.gson.Gson;

/**
 * @author elirandr
 *
 */
public class AlarmClock {

	private static Properties config = new Properties();
	private static AlarmLcd lcdScreen;
	private static AlarmBuzzer buzzer;
	private static GroveRotary rotary;
	private static float lastRotary = -1;//last rotary sensor value
	private static GroveButton button;
	private static DateTime currentTime = new DateTime();
	private static DateTime alarmTime;//Scheduled alert time
	private static Timer alarmTimerObj = new Timer();
	private static boolean alarmActive = false;
	private static boolean tick = true;//for the alarm on-off sound

	/**
	 * initialize sensors values
	 */
	public static void initSensors(){
		lcdScreen = new AlarmLcd();
		buzzer = new AlarmBuzzer(5);
		rotary = new GroveRotary(0);
		button = new GroveButton(4);
	}
	
	/**
	 * Start the clock, then check every 50ms to see if is time to
	 * turn on the alarm 
	 */
	public static void startClock() {
		Timer clockTimer = new Timer();
		DateTimeFormatter dateFormat = DateTimeFormat.forPattern("H:mm:ss a");
		clockTimer.schedule(new TimerTask() {
			public void run() {
				DateTime date = new DateTime();
				// check if display needs to be updated
				if (date.isAfter(currentTime)){
					lcdScreen.displayMessageOnLcd(dateFormat.print(date) ,0);
					if (alarmTime != null && !alarmActive){//check if alarm has been set
						if ((currentTime.withMillisOfSecond(0)).isEqual(alarmTime.withMillisOfSecond(0))) //check equality to seconds level
							startAlarm();
					}
				}
				currentTime = date;
			}
		}, 50,50);
	}
	
	/**
	 * Called to start the alarm when the time has come to get up
	 */
	private static void startAlarm() {
		lcdScreen.setLcdColor("red");	
		buzzer.buzz();
		String weather = "";
		try {
			weather = Utils.getWeather(config);//get weather of location set in config file
		} catch (IOException e) {
			System.out.println("unable to get weather data");
			e.printStackTrace();
		}
		lcdScreen.displayMessageOnLcd(weather, 1);
		alarmActive = true;
		alarmTimerObj.schedule(new TimerTask() {
			public void run() {
				lcdScreen.setLcdColor(tick ? "white" : "red");//every 250ms will change LCD color from red to white
				if (tick){ 
					buzzer.stopBuzzing(); 
				} 
				else { 
					buzzer.buzz(); 
				}
				tick = !tick;
			}
		}, 250, 250);
	}

	/**
	 * Loops every 100ms to check latest value of the rotary dial, 
	 * so we can check if has been moved and adjust LCD brightness
	 * Also checks to see if the button was pressed, so we can fire
     * stop the alarm timer
	 */
	public static void checkSensorsActivity() {
		Timer activityCheckTimer = new Timer();
		activityCheckTimer.schedule(new TimerTask() {
			public void run() {
				if (rotary.abs_value() != lastRotary){//check if rotary dial changed
					lastRotary = rotary.abs_value();
					lcdScreen.adjustBrightness(lastRotary);
				}
			}
		}, 100,100);
		
		button.installISR(2,new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				System.out.println("button clicked");
				alarmTimerObj.cancel();
				alarmTimerObj = new Timer();
				// notify how long alarm took to be silenced
				Period interval = new Period(alarmTime, (new DateTime())); 
				Utils.notifyAzure(String.valueOf(interval.toDurationFrom(new DateTime()).getMillis()), config);
				
				alarmTime.plusDays(1);
				lcdScreen.setLcdColor("white");
				buzzer.stopBuzzing();
				alarmActive = false;
			}
		});
	}
	
	
	/** 
	 * Starts the built-in web server that serves up the web page
	 * used to set the alarm time
	 */
	public static void setupServer() {
		ServerSetup server= new ServerSetup();
		server.setupServer(8080);
		// Set new alarm time submitted by the web page using HTTP GET
		server.addServlet("/", new ServerSetup.GetCall(){
			@Override
			public void runCall(HttpServletRequest request, HttpServletResponse response) throws IOException {
				if (request.getRequestURI().matches("/")){
					Map<String, String[]> params = request.getParameterMap();
					DateTime time = new DateTime();
					if (params.containsKey("hour")&& params.containsKey("minute")&&params.containsKey("second")){
						time = time.withTime(Integer.parseInt(request.getParameter("hour")),
								Integer.parseInt(request.getParameter("minute")), 
								Integer.parseInt(request.getParameter("second")),
								0);
						if (time.isBefore(new DateTime())){
							time.plusDays(1);
						}		
						alarmTime = time;
					}
					String sCurrentLine;
					@SuppressWarnings("resource")
					BufferedReader indexFile = new BufferedReader(new FileReader("/var/AlarmClock/index.html"));
					StringBuilder stringBuilder = new StringBuilder();
					while ((sCurrentLine = indexFile.readLine()) != null) {
						stringBuilder.append(sCurrentLine).append("\n");
					}
					response.getWriter().println(stringBuilder.toString());
				}
			}  
		});
		// Return the JSON data for the currently set alarm time
		server.addServlet("/alarm.json", new ServerSetup.GetCall(){
			public void runCall(HttpServletRequest request, HttpServletResponse response) throws IOException {
				Map<String, Integer> alarmMap = new HashMap<String, Integer>();
				if (alarmTime != null){
					alarmMap.put("hour", alarmTime.getHourOfDay());
					alarmMap.put("minute", alarmTime.getMinuteOfHour());
					alarmMap.put("second", alarmTime.getSecondOfMinute());
				}
				else{
					alarmMap.put("hour", 0);
					alarmMap.put("minute", 0);
					alarmMap.put("second", 0);
				}
				//System.out.println(alarmMap.get("hour") + "|" + alarmMap.get("minute") + "|" + alarmMap.get("second"));
				response.setContentType("application/json");
				response.setStatus(HttpServletResponse.SC_OK);
				Gson gson = new Gson();
				String result =gson.toJson(alarmMap);
				response.getWriter().println(result);
			}  
		});

		server.run();
	}

	public static void main(String[] args) {
		// check that we are running on Galileo or Edison
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
			config.load(AlarmClock.class.getClassLoader().getResourceAsStream("config.properties"));
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		initSensors();
		buzzer.stopBuzzing();
		startClock();
		checkSensorsActivity();
		setupServer();
	}
}
