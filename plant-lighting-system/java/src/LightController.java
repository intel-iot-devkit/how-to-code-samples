
/**
 * helper class for controlling light sensor
 * @author staite
 * 
 */
public class LightController {

	upm_grove.GroveLight gl ;
	public LightController(int slot){
		gl = new upm_grove.GroveLight(slot);
	}
	
	/**
	 * get current light level detected
	 * @return float light level detected
	 * 
	 */
	public float getCurrentValue(){
		return gl.value();
	}
}
