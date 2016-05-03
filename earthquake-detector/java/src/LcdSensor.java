import java.util.Properties;


/**
 * 
 */

/**
 * @author rhassidi
 *
 */
public class LcdSensor {
    
    private upm_i2clcd.Jhd1313m1 lcd;
    private Properties config;
    
    private final short RED[] = {255,0,0};
    private final short GREEN[] = {0,255,0};
    private final short WHITE[] = {255,255,255};

    /**
     * The constructor of the LcdSensor that initializes the lcd sensor
     * 
     * @param lcdPort The port the the lcd is connected to
     * @param config The loaded config file
     */
    public LcdSensor(int lcdPort, Properties config){
	this.lcd = new upm_i2clcd.Jhd1313m1(lcdPort, 0x3E, 0x62);
	this.config = config;
    }

    /**
     * Writes the given message on the screen, and colors the screen with the wanted color
     * 
     * @param message The message to write on the lcd screen
     */
    public void writeMessage(String message){
	short color[] = (message.equals("Earthquake!!!"))? RED : ((message.equals("stopping")) ? WHITE : GREEN);
	
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.setColor(color[0], color[1], color[2]);
	lcd.write(message);

	Utils.notifyAzure(config, message);
	System.out.println("color: " + color + " message: " + message);
    }
}
