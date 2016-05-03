
import java.io.IOException;
import java.util.Timer;
import java.util.TimerTask;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.google.gson.Gson;
import com.google.gson.JsonArray;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;


/**
 * 
 * @author rhassidi
 *
 */
public class FieldDataReporter {

    private static upm_i2clcd.SSD1308 oled;
    private static upm_at42qt1070.AT42QT1070 button;
    private static upm_bmpx8x.BMPX8X atmospherePressureSensor;


    private static Gson gson;
    private static JsonArray allSensorsData;

    /**
     * Initializes sensors
     */
    private static void initSensors(){
//	button = new upm_at42qt1070.AT42QT1070(upm_at42qt1070.javaupm_at42qt1070.AT42QT1070_I2C_BUS, (short)0x2e);
	atmospherePressureSensor = new upm_bmpx8x.BMPX8X(0, 0x77);
	oled = new upm_i2clcd.SSD1308(0);
	allSensorsData = new JsonArray();
    }


    /**
     * Loops every 1000ms to calculate atmosphere pressure and update the Json 
     * data structure "allSensorsData" with the properties "pressure", "temp",
     * "altitude" and "sealevel".
     */
    public static void monitorAtmospherePressure(){


	Timer monitorTimer = new Timer();
	monitorTimer.schedule(new TimerTask() {
	    public void run() {
		JsonObject currData = new JsonObject();

		currData.addProperty("pressure", atmospherePressureSensor.getPressure());
		currData.addProperty("temp", atmospherePressureSensor.getTemperature());
		currData.addProperty("altitude", atmospherePressureSensor.getAltitude());
		currData.addProperty("sealevel", atmospherePressureSensor.getSealevelPressure());

		System.out.println(currData);

		allSensorsData.add(currData);
	    }
	}, 1000, 1000);
    }

    /**
     * Presents the latest measurement of the atmosphere on the OLED screen.
     */
    public static void showLatest(){
	String currData = gson.fromJson(allSensorsData.get(allSensorsData.size()-1), String.class);
	oled.clear();
	oled.setCursor(0, 0);

	oled.write(currData);
	System.out.println(currData);

    }

    /**
     * Loops every 100ms to check if the button was pressed.
     * If the button was pressed, "showLatest()" is called.
     */
    public static void button(){
	boolean prev = false;


	Timer monitorTimer = new Timer();
	monitorTimer.schedule(new TimerTask() {
	    public void run() {

		button.updateState();
		boolean pressed = false;
		int buttons = button.getButtons();

		for(int i = 0 ; i < 7 ; i++){
		    if((buttons & (1 << i)) != 0){
			pressed = true;
		    }
		    if(!prev && pressed){
			showLatest();
		    }
		}
	    }
	}, 100, 100);
    }

    /**
     * Starts the built-in web server that serves up the web page.
     * Used to show the captures measurements from the atmosphere pressure sensor.
     */
    public static void setupServer() {
	ServerSetup server = new ServerSetup();
	server.setupServer(8080);
	// Get the "datas" Json object from the server by sending an HTTP GET request
	server.addServlet("/", new ServerSetup.GetCall(){
	    @Override
	    public void runCall(HttpServletRequest request, 
		    HttpServletResponse response) throws IOException {
		if (request.getRequestURI().matches("/")){

		    response.setContentType("application/json");
		    response.setStatus(HttpServletResponse.SC_OK);

		    for(JsonElement data : allSensorsData){
			response.getWriter().println(data);
		    }
		}
	    }  
	});

	server.run();
    }

    /**
     * 
     */
    public static void main(String[] args) {
	initSensors();

	monitorAtmospherePressure();
	setupServer();
	button();
    }

}