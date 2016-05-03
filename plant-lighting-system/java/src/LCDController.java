import upm_i2clcd.Jhd1313m1;


/**
 * helper class for controlling LCD
 * @author staite
 *
 */
public class LCDController {
	public static Jhd1313m1 lcdScreen;
	public LCDController(int slot) {
		lcdScreen = new Jhd1313m1(slot);
	}
	
	/**
	 * Displays a message on the RGB LCD
	 * @param string- message
	 * @param line- which line on the LCD
	 */
	public void displayMessageOnLcd(String string, int line) {
		// pad string to avoid display issues
		while (string.length() < 16) { string += " "; }
		lcdScreen.setCursor(line, 0);
		lcdScreen.write(string);
	}
}
