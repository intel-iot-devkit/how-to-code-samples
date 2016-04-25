import upm_i2clcd.Jhd1313m1;


public class AlarmLcd {
	public static Jhd1313m1 lcdScreen;
	// Colors used for the RGB LCD
	private static final short[] redColor = {255, 0, 0}; 
	private static final short[] whiteColor = {255, 255, 255}; 
	
	public AlarmLcd() {
		lcdScreen = new Jhd1313m1(1, 0x3E, 0x62);
		displayMessageOnLcd("",1);
	}
	
	// Sets the background color on the RGB LCD
	/**
	 * @param colorString color for the LCD background
	 */
	public void setLcdColor(String colorString) {
		switch (colorString){
		case "red": 	lcdScreen.setColor(redColor[0], redColor[1], redColor[2]); break;
		case "white": 	lcdScreen.setColor(whiteColor[0], whiteColor[1], whiteColor[2]); break;
		}
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
