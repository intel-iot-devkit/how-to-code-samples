import java.io.IOException;

import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpUriRequest;
import org.apache.http.client.methods.RequestBuilder;
import org.apache.http.impl.client.HttpClients;

import upm_buzzer.Buzzer;
import upm_i2clcd.*;
import upm_ttp223.TTP223;

public class Doorbell {

	static short[] colour_white = { 255, 255, 255 };
	static short[] colour_green = { 0, 255, 0 };

	static boolean done = false;

	private static Jhd1313m1 lcd = null;
	private static Buzzer buzzer = null;
	private static TTP223 touch = null;

	static class Dingdong implements Runnable {
		public void run() {
			if (touch.isPressed()) {

				Thread t = new Thread(incrementRunnable);
				t.start();

				message("dingdong", colour_green);
				synchronized (buzzer) {
					buzzer.playSound(2600, 0);
				}
			} else {
				reset();
			}
		}
	}

	static void reset() {
		message("doorbot ready", null);

		synchronized (buzzer) {
			buzzer.setVolume(0.5f);
			buzzer.stopSound();
			buzzer.stopSound();
		}
	}

	static String server = "http://intel-examples.azurewebsites.net/logger/doorbell";
	static String auth = "s3cr3t";

	static Runnable incrementRunnable = new Runnable() {

		@Override
		public void run() {
			HttpClient client = HttpClients.createMinimal();
			HttpUriRequest request = RequestBuilder.put().setUri(server)
					.setHeader("X-Auth-Token", auth).build();
			try {
				client.execute(request);
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	};

	static synchronized void message(String message, short[] colour) {
		while (message.length() < 16)
			message += " ";
		lcd.setCursor(0, 0);
		lcd.write(message);
		if (colour == null || colour.length != 3)
			colour = colour_white;

		lcd.setColor(colour[0], colour[1], colour[2]);
	}

	public static void main(String[] args) {

		if (args.length >= 2) {
			server = args[0];
			auth = args[1];
		} else {
			String configuration = String.format(
					"Default configuration: SERVER = '%s', AUTH = '%s'",
					server, auth);
			System.out.println(configuration);
			System.out
					.println("Provide configuration as parameters: <server> <auth>");
		}

		lcd = new Jhd1313m1(0);
		buzzer = new Buzzer(5);
		touch = new TTP223(4);
		lcd.displayOn();
		lcd.clear();

		reset();

		Dingdong dingdong = new Dingdong();
		touch.installISR(mraa.Edge.EDGE_BOTH.swigValue(), dingdong);
		while (!done) {
			try {
				Thread.sleep(1000);
			} catch (InterruptedException e) {

			}
		}
	}
}