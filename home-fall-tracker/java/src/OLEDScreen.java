import upm_i2clcd.SSD1308;



/**
 * helper class for OLED screen usage
 * @author staite
 *
 */
public class OLEDScreen {
	public static SSD1308 oledScreen;

	public OLEDScreen() {
		oledScreen = new SSD1308(0,0x3C);
		oledScreen.clear();
	}


	/**
	 * Displays a message on the OLED LCD
	 * @param string- message
	 */
	public void displayMessageOnOLED(String message) {
		// pad string to avoid display issues
		oledScreen.clear();
		oledScreen.home();
		oledScreen.write(message);
	}




}
