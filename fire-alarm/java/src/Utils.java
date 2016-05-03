import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.Properties;

import com.google.gson.JsonObject;

public class Utils {


	/**
	 * Display and then store record in the remote datastore
	 * of the time of the fire
	 * @param value - the time of the fire  to send to azure 
	 * @param config properties file which include AUTH_TOKEN
	 */
	public static void notifyAzure(String value, Properties config) {
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
					System.out.println("problem posting data to azure");
				
			}
		}});
		thread.run();
		
	}
	
	/**
	 * send sms using Twilio API
	 * @param body String message
	 * @param config Properties configuration file object
	 */
	public static void sendMessageWithTwilio(String body, Properties config){
		final String ACCOUNT_SID = config.getProperty("TWILIO_ACCT_SID");
		final String AUTH_TOKEN = config.getProperty("TWILIO_AUTH_TOKEN");
		final String TWILIO_OUTGOING_NUMBER = config.getProperty("TWILIO_OUTGOING_NUMBER");
		final String NUMBER_TO_SEND_TO = config.getProperty("NUMBER_TO_SEND_TO");

//		TwilioRestClient client = new TwilioRestClient(ACCOUNT_SID, AUTH_TOKEN); 
//		final Account mainAccount = client.getAccount();
//		final SmsFactory messageFactory = mainAccount.getSmsFactory();
//		List<NameValuePair> params = new ArrayList<NameValuePair>(); 
//		params.add(new BasicNameValuePair("To", NUMBER_TO_SEND_TO)); 
//		params.add(new BasicNameValuePair("From", TWILIO_OUTGOING_NUMBER)); 
//		params.add(new BasicNameValuePair("Body", body));   
//		try {
//			messageFactory.create(params);
//		} catch (TwilioRestException e) {
//			// TODO Auto-generated catch block
//			System.out.println("problem sending sms");
//			e.printStackTrace();
//		}


//		TwilioRestClient client = new TwilioRestClient(ACCOUNT_SID, AUTH_TOKEN); 
//		List<NameValuePair> params = new ArrayList<NameValuePair>(); 
//		params.add(new BasicNameValuePair("To", NUMBER_TO_SEND_TO)); 
//		params.add(new BasicNameValuePair("From", TWILIO_OUTGOING_NUMBER)); 
//		params.add(new BasicNameValuePair("Body", body));   
//
//		MessageFactory messageFactory = client.getAccount().getMessageFactory(); 
//		Message message = null;
//		try {
//			message = messageFactory.create(params);
//		} catch (TwilioRestException e) {
//			// TODO Auto-generated catch block
//			System.out.println("problem sending sms message using twilio");
//			e.printStackTrace();
//		} 
//		System.out.println(message.getSid()); 
	}
}
