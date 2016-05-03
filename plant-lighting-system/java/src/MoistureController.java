import java.util.AbstractMap;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;





import org.joda.time.DateTime;



/**
 * helper class for moisture sensor
 * @author staite
 *
 */
public class MoistureController {

	private upm_grovemoisture.GroveMoisture gm;
	private List<Map.Entry<DateTime,Integer>> valueLogger;
	
	/**
	 * initializes moisture sensor
	 * @param slot sensor slot
	 */
	public MoistureController(int slot){
		
		gm = new upm_grovemoisture.GroveMoisture(slot);
		valueLogger = new ArrayList<Map.Entry<DateTime,Integer>>();
	}
	
	/**
	 * gets current moisture level value
	 * @return int moisture level
	 */
	public int getCurrentMoistureValue(){
		int currentValue = gm.value();
		System.out.println("moisture level: " + currentValue);
		if(valueLogger.size() > 20){
			valueLogger.remove(0);
		}
		valueLogger.add(new AbstractMap.SimpleImmutableEntry<DateTime,Integer>(new DateTime(),currentValue));
		return currentValue;
	}
	
	/**
	 * get all values previously logged for moisture levels
	 * @return List<Map.Entry<DateTime,Integer>> all values previously logged for moisture levels
	 */
	public List<Map.Entry<DateTime,Integer>> getAllLoggedMoistureValues(){
		return valueLogger;
	}
	
	
	/**
	 * creates HTML representation of all previously logged moisture levels
	 * @return String HTML representation of all previously logged moisture levels
	 */
	public String getHTMLForAllLoggedMoistureValues(){
		StringBuilder result = new StringBuilder("");
		for(Map.Entry<DateTime,Integer> moistureDateAndValue : valueLogger){
			result.append("<tr>").append("\n");
			result.append("<td>").append("\n");
			result.append(moistureDateAndValue.getKey().toDateTimeISO()).append("\n");
			result.append("</td>").append("\n");
			result.append("<td>").append("\n");
			result.append(moistureDateAndValue.getValue()).append("\n");
			result.append("</td>").append("\n");
			result.append("</tr>").append("\n");
		}
		return result.toString();
	}
}
