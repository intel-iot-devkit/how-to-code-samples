import upm_buzzer.Buzzer;

public class AlarmBuzzer {
	private Buzzer buzzer;

	public AlarmBuzzer(int slot) {
		buzzer = new Buzzer(slot);
		stopBuzzing();
	}
	
	/**
	 * start buzzer
	 */
	public void buzz() {//change to toggle buzz
		buzzer.setVolume(0.5f);
		buzzer.playSound(2600, 0);
	}

	/**
	 * stop buzzer
	 */
	public void stopBuzzing() {
		buzzer.stopSound();
		buzzer.stopSound(); // if called only once, buzzer doesn't completely stop
	}
	}
