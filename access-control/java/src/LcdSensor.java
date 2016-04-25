import upm_i2clcd.Jhd1313m1;

/**
 * @author rhassidi
 *
 */
public class LcdSensor {


    private upm_i2clcd.Jhd1313m1 lcd;

    private final short BLUE[] = {0,0,255};
    private final short RED[] = {255,0,0};
    private final short WHITE[] = {255,255,255};
    
    

    /**
     * @param lcdPort The port the the lcd is connected to
     * @param config The loaded config file
     */
    public LcdSensor(int lcdPort){
	this.lcd = new Jhd1313m1(lcdPort, 0x3E, 0x62);
    }

    

    /**
     * Writes the given message on the screen, and colors the screen with the wanted color
     * @param message The message to write on the lcd screen
     */
    public void writeMessage(String message){
	short color[] = (message.equals("ALARM"))? RED : ((message.equals("Alert")) ? BLUE : WHITE);
	
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.setColor(color[0], color[1], color[2]);
	lcd.write(message);

	Utils.NotifyAzure(message);
    }
}
