import mraa.Gpio;

/**
 * water pump helper library
 * @author staite
 *
 */
public class WaterPump {
	private Gpio pump;

	public WaterPump(){
		pump = new Gpio(4);
	}

	/**
	 * turn on water pump
	 */
	public void turnOn(){
		pump.write(1);
	}

	/**
	 * turn off water pump
	 */
	public void turnOff(){
		pump.write(0);
	}
}
