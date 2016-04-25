import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.Properties;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class AccessControl {

    private static Properties config;
    private static MotionActivityHandling motionActivityHandling;   


    /**
     * Initializes motion activity handling object that constructs the lcd and the 
     * motion sensor instances
     */
    private static void initSensors() {
	motionActivityHandling = new MotionActivityHandling();
    }

    /**
     * Loads the main config file for future use
     */
    private static void loadConfigFile() {
	try {
	    config = Utils.loadConfig();
	} catch (IOException e) {
	    e.printStackTrace();
	}
    }

    /** 
     * Starts the built-in web server that serves up the web page
     * used to enter code after triggering the motion sensor
     */
    public static void setupServer() {
	ServerSetup server= new ServerSetup();
	server.setupServer(8080);

	// Set new alarm time submitted by the web page using HTTP GET
	server.addServlet("/", new ServerSetup.GetCall(){
	    @Override
	    public void runCall(HttpServletRequest request, HttpServletResponse response) throws IOException {
		if (request.getRequestURI().matches("/")){

		    String sCurrentLine;
		    @SuppressWarnings("resource")
		    BufferedReader indexFile = new BufferedReader(new FileReader("/var/AccessControl/index.html"));
		    StringBuilder stringBuilder = new StringBuilder();
		    while ((sCurrentLine = indexFile.readLine()) != null) {
			stringBuilder.append(sCurrentLine).append("\n");
		    }
		    response.getWriter().println(stringBuilder.toString());
		}
	    }  
	});

	// Stop alarm if the given code matches the code in the config file
	server.addServlet("/alarm", new ServerSetup.GetCall(){
	    public void runCall(HttpServletRequest request, HttpServletResponse response) throws IOException {
		//		System.out.println("motion.isExpectingCode(): "+motionSensor.isExpectingCode());
		//		System.out.println("req.getParam('code'): "+request.getParameter("code"));
		//		System.out.println("config.getProperty('CODE'): "+ codeConfig.getProperty("CODE"));
		if(motionActivityHandling.isExpectingCode() && 
			(request.getParameter("code").equals(config.getProperty("CODE")))){
		    motionActivityHandling.setValidatedTrue();
		}
	    }
	});

	server.run();
    }

    /**
     *  The main function calls `server()` to start up
     *  the built-in web server used to enter the access code
     * after triggering the alarm.
     * It also calls the `lookForMotion()` function which monitors
     */
    public static void main(String[] args) {
	loadConfigFile();
	initSensors();
	setupServer();
	motionActivityHandling.lookForMotion();

    }
}