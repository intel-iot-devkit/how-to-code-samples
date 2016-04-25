import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.Properties;

import com.google.gson.JsonObject;

public class Utils {


	/**
	 * Store record in the remote datastore when robot is active
	 * @param value - the current time
	 * @param config properties file which include AUTH_TOKEN
	 */
	public static void notifyAzure(String value, Properties config){

		Thread thread = new Thread(new Runnable() {

			@Override
			public void run() {
				// TODO Auto-generated method stub
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
					jsonObject.addProperty("value", value);
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
					System.out.println("problem sending data to Azure");

				}
			}
		});
		thread.run();


	}

}
