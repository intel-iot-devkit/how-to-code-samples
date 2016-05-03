import upm_i2clcd.SSD1308;

/**
 * OLED display helper class
 * @author staite
 *
 */
public class OLEDScreen {

	private SSD1308 oledScreen ;
	public OLEDScreen(){
		oledScreen = new SSD1308(0, 0x3C);
		oledScreen.clear();
	}
	public synchronized  void writeToScreen(String message){
		oledScreen.clear();
		oledScreen.write(message);
	}
}
