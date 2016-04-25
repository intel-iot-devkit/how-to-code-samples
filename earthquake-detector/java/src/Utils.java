import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.Date;
import java.util.Properties;

//import org.joda.time.DateTime;




import com.google.gson.Gson;
import com.google.gson.JsonObject;

public class Utils {

    /**
     * Checks with the USGS API if an earthquake actualy took place
     * 
     * @param config The config file that contains the wanted values as parameters
     * in the URL
     * @return a String representation of the real earthquake status
     * @throws IOException
     */
    @SuppressWarnings("deprecation")
    public static String getRealEarthquakeStatus(Properties config) throws IOException {
	Date time = new Date();
	time.setMinutes(time.getMinutes() - 10);;

	String url = config.getProperty("QUAKE_SERVER");
	url += "?format=geojson";
	url += "&starttime="+time.toString().replace(" ", "%20");
	url += "&latitude="+config.getProperty("LATITUDE");
	url += "&longitude="+config.getProperty("LONGITUDE");
	url += "&maxradiuskm="+config.getProperty("MAX_RADIUS_KM");
	String earthquake = "";
	HttpURLConnection httpConnection = (HttpURLConnection) (new URL(url)).openConnection();
	httpConnection.setRequestMethod("GET");
	httpConnection.setRequestProperty("User-Agent", "Mozilla/5.0");

//	System.out.println("http get req: "+httpConnection.getURL());
	
//	try {
//	    Thread.sleep(2000);
//	} catch (InterruptedException e) {
//	    e.printStackTrace();
//	}
	
	int HttpResult = httpConnection.getResponseCode(); 
	//System.out.println("status of call: " + HttpResult);
	if(HttpResult == HttpURLConnection.HTTP_OK){
	    BufferedReader in = new BufferedReader(new InputStreamReader(httpConnection.getInputStream()));
	    String inputLine;
	    StringBuffer response = new StringBuffer();
	    while ((inputLine = in.readLine()) != null) {
		response.append(inputLine);
	    }
	    in.close();
	    System.out.println(response.toString());
	    Gson gson = new Gson();
	    JsonObject jobj = gson.fromJson(response.toString(), JsonObject.class);
	    if (jobj.getAsJsonArray("features").size() > 0){
		System.out.println("An earthquake has accored in the area:" + earthquake);
		earthquake =  "Earthquake!!!";
	    }
	    else{
		earthquake = "No quake";
	    }

	    httpConnection.disconnect();
	}
	return earthquake;


    }

    /**
     * Store record in the remote datastore when the accelerometer detects an earthquake
     * 
     * @param message The message to be sent to the Azure server
     * @param config The config file the contains the servers address and password
     */
    public static void notifyAzure(Properties config, String message){
	Thread thread = new Thread(new Runnable() {

	    @Override
	    public void run() {
		try {
		    String url = config.getProperty("SERVER");
		    HttpURLConnection httpConnection = (HttpURLConnection) (new URL(url)).openConnection();
		    httpConnection.setRequestMethod("PUT");
		    httpConnection.setDoOutput(true);
		    httpConnection.setDoInput(true);
		    httpConnection.setRequestProperty("Content-Type", "application/json");
		    httpConnection.setRequestProperty("Accept", "application/json");
		    httpConnection.setRequestProperty("X-Auth-Token", config.getProperty("AUTH_TOKEN"));
		    JsonObject jsonObject = new JsonObject();
		    jsonObject.addProperty("value", message + " on " + new Date().toString());
		    OutputStreamWriter wr = new OutputStreamWriter(httpConnection.getOutputStream());
		    wr.write(jsonObject.toString());
		    wr.flush();
		    int HttpResult = httpConnection.getResponseCode(); 
		    //System.out.println("status of call: " + HttpResult);
		    if(HttpResult != HttpURLConnection.HTTP_OK){
			StringBuilder sb = new StringBuilder();  
			BufferedReader br = new BufferedReader(new InputStreamReader(httpConnection.getInputStream(),"utf-8"));  
			String line = null;  
			while ((line = br.readLine()) != null) {  
			    sb.append(line + "\n");  
			}  
			br.close();  
			System.out.println(""+sb.toString());  
		    }
		} catch (Exception e) {
		    e.printStackTrace();
		    System.out.println("problem posting data to azure");
		}
	    }
	});
	thread.start();

    }
}
