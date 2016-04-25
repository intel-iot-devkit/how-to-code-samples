import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.Properties;

import com.google.gson.Gson;
import com.google.gson.JsonObject;

public class Utils {

	/**
	 * Call the remote Weather Underground API to check the weather conditions
	 * change the LOCATION variable to set the location for which you want.
	 * @param config properties file which include LOCATION and WEATHER_API_KEY
	 * @return weather string from wunderground
	 * @throws IOException
	 */
	public static String getWeather(Properties config) throws IOException {
		String url = "http://api.wunderground.com/api/";
		url += config.getProperty("WEATHER_API_KEY");
		url += "/conditions/q/CA/" + config.getProperty("LOCATION") + ".json";
		String weather = "";
		HttpURLConnection httpConnection = (HttpURLConnection) (new URL(url)).openConnection();
		httpConnection.setRequestMethod("GET");
		httpConnection.setRequestProperty("User-Agent", "Mozilla/5.0");
		BufferedReader in = new BufferedReader(new InputStreamReader(httpConnection.getInputStream()));
		String inputLine;
		StringBuffer response = new StringBuffer();
		while ((inputLine = in.readLine()) != null) {
			response.append(inputLine);
		}
		in.close();
		//System.out.println(response.toString());
		Gson gson = new Gson();
		JsonObject jobj = gson.fromJson(response.toString(), JsonObject.class);
		weather = jobj.get("current_observation").getAsJsonObject().get("weather").getAsString();
		System.out.println("forecast:" + weather);
		return weather;
	}

	/**
	 * Display and then store record in the remote datastore
	 * of how long the alarm was ringing before it was turned off
	 * @param duration - duration of the timer to send to azure 
	 * @param config properties file which include AUTH_TOKEN
	 */
	public static void notifyAzure(String duration, Properties config){
		Thread thread = new Thread(new Runnable() {

			@Override
			public void run() {
				// TODO Auto-generated method stub
				try {
					System.out.println("Alarm duration:" + duration);
					String url = config.getProperty("SERVER");
					HttpURLConnection httpConnection = (HttpURLConnection) (new URL(url)).openConnection();
					httpConnection.setRequestMethod("PUT");
					httpConnection.setDoOutput(true);
					httpConnection.setDoInput(true);
					httpConnection.setRequestProperty("Content-Type", "application/json");
					httpConnection.setRequestProperty("Accept", "application/json");
					httpConnection.setRequestProperty("X-Auth-Token", config.getProperty("AUTH_TOKEN"));
					JsonObject jsonObject = new JsonObject();
					jsonObject.addProperty("value", duration);
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
					// TODO Auto-generated catch block
					e.printStackTrace();
					System.out.println("problem posting data to azure");
				}
			}
		});
		thread.start();

	}
}
