import upm_mma7660.MMA7660;


/**
 * @author rhassidi
 *
 */
public class AccelerometerSensor {
    private upm_mma7660.MMA7660 accelerometer;

    /**
     * Initializes the accelerometer sensor and constructs the AccelerometerSensor object
     * 
     * @param accelerometerPort The port that the 3-axis sensor is connected to
     */
    public AccelerometerSensor(int accelerometerPort){
	accelerometer = new upm_mma7660.MMA7660(accelerometerPort);
	enable64samplesPerSec();
    }

    /**
     * A public getter that returns the accelerometer instance
     * 
     * @return the accelerometer instance
     */
    public MMA7660 getAccelerometer(){
	return accelerometer;
    }

    /**
     * Enables the 3-axis sensor to take 64 samples per sec
     */
    private void enable64samplesPerSec(){
	//	accelerometer.setModeStandby();
	//	accelerometer.setSampleRate(upm_mma7660.MMA7660.MMA7660_AUTOSLEEP_T.AUTOSLEEP_64);
	//	accelerometer.setModeActive();
    }
}
