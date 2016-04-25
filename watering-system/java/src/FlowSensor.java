import java.util.Properties;
import java.util.Timer;
import java.util.TimerTask;

/**
 * Flow sensor helper library
 * @author staite
 *
 */
public class FlowSensor {
	private  upm_grovewfs.GroveWFS flow ;

	public FlowSensor(){
		flow = new upm_grovewfs.GroveWFS(2);
	}

	/**
	 * for current hour expected status of pump (on or off) check if flow sensor status
	 * fits the expected and alert if not(send message using Twilio service)
	 * @param shouldWaterPumpBeOn - expected pump status
	 * @param config - configuration file object
	 */
	public void alertIfFlowStateDoesntMatchSetting(boolean shouldWaterPumpBeOn, Properties config){
		flow.clearFlowCounter();
		flow.startFlowCounter();
		Timer timer = new Timer();
		timer.schedule(new TimerTask() {

			@Override
			public void run() {
				// TODO Auto-generated method stub
				if(flow.flowRate() >= 0.5 &&  !shouldWaterPumpBeOn){ //pump should be off but there is flow
					Utils.sendMessageWithTwilio("watering system alarm", config);
				}
				else if(flow.flowRate() < 1 &&  shouldWaterPumpBeOn){ //pump should be on but there is no flow
					Utils.sendMessageWithTwilio("watering system alarm", config);
				}

			}
		}, 2000);

	}
}
