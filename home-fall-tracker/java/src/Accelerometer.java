import upm_adxl345.Adxl345;


public class Accelerometer {
	private Adxl345 accel;
	public Accelerometer(){
        accel = new Adxl345(0);
        
	}
	public float[] getCurrentForceValues(){
		accel.update();
		return accel.getAcceleration();
	}
}
