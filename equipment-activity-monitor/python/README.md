# Equipment Activity Monitor in Python*

## What it is

Using a compatible Intel® IoT Platform, this project lets you create a shop-floor equipment activity monitor that:<br>
- tracks equipment usage by monitoring sound and vibration sensors.<br>
- issues a visual notification whenever the equipment is in use.<br>
- logs equipment usage using cloud-based data storage.

## First time setup  
For all the samples in this repository, see the ![General Setup Instructions](./../../README.md#setup) for required boards and libraries. You need either Grove or DFRobot sensors but not both.

## Hardware requirements

### Grove\*

You need to have a Grove\* Shield connected to an Arduino\* compatible breakout board to plug all the Grove\* devices into the Grove\* Shield. Make sure you have the tiny VCC switch on the Grove\* Shield set to **5V**.

Sensor | Pin
--- | ---
Grove\* Sound Sensor | A0
Grove\* Piezo Vibration Sensor | A2
Grove\* RGB LCD | I2C

### DFRobot\*

You need to have a LCD Keypad Shield connected to an Arduino\* compatible breakout board to plug all the DFRobot\* devices into the LCD Keypad Shield.

Sensor | Pin
--- | ---
Analog Sound Sensor | A1
Digital Vibration Sensor | A2

More details on the hardware requirements can be found in the ![project README](./../README.md)

## Software requirements

1. ![MRAA](https://github.com/intel-iot-devkit/mraa) and ![UPM](https://github.com/intel-iot-devkit/upm) 
2. Python 2.7
3. Packages as listed in the ![setup.py](setup.py) file in this directory
4. Microsoft Azure\*, IBM Bluemix\*, AT&T M2X\*, AWS\*, Predix\*, or SAP\* account (optional)

You can install the dependencies 

### Python\* Package Manager (pip)

To install the Python\* package manager needed to install and run the example:

    $ pip install --upgrade pip setuptools

### Install the example

Once all dependencies are installed you can install the example itself with the following command:

    $ pip install --src ~/python/examples/ -e "git+https://github.com/intel-iot-devkit/how-to-code-samples.git#egg=iot_equipment_activity_monitor&subdirectory=equipment-activity-monitor/python"

The `pip` command will install required Python dependencies, save the source code for the example in `~/python/examples/iot_equipment_activity_monitor/` and link the package to the global Python `site-packages` folder.

## Configuring the example

When the example is installed through `pip` the `config.json` file that holds the configuration for the example lives in `~/python/examples/iot_equipment_activity_monitor/equipment-activity-monitor/python/iot_equipment_activity_monitor/config.json`.

To configure the example for the Grove\* kit, just leave the `kit` key in the `config.json` set to `grove`. To configure the example for the DFRobot\* kit, change the `kit` key in the `config.json` to `dfrobot` as follows:

```JSON
{
  "kit": "dfrobot",
  "VIBRATION_THRESHOLD": 100,
  "NOISE_THRESHOLD": 0
}
```

To configure the example for the Arduino 101, add a `PLATFORM` key with the value `firmata` to the `config.json`, as follows:

```JSON
{
  "kit": "grove",
  "PLATFORM": "firmata",
  "VIBRATION_THRESHOLD": 100,
  "NOISE_THRESHOLD": 0
}
```

To configure the example for the optional Microsoft Azure\*, IBM Bluemix\*, or AWS\* data store, add the `SERVER` and `AUTH_TOKEN` keys in the `config.json` file below as follows:

```JSON
{
  "kit": "grove",
  "VIBRATION_THRESHOLD": 100,
  "NOISE_THRESHOLD": 0,
  "SERVER": "http://intel-examples.azurewebsites.net/logger/access-control",
  "AUTH_TOKEN": "s3cr3t"
}
```

## Running the program

Once the example is installed through `pip` you can run the program by running the following command in an SSH session to the board:

    $ python -m iot_equipment_activity_monitor

You will see output similar to below when the program is running.

```
Noise level: 305
Value: start 2016-04-22T02:54:15.946Z 
Connecting to MQTT server... 
MQTT message published: { d: { value: 'start 2016-04-22T02:54:15.951Z' } }
Vibration: 48 
```
The LCD should now display `ready`.<br>
![](./../images/equipment-lcd.jpg)

Refer to ![How it Works](./../README.md#how-it-works) for details on the functionality.

### IoT cloud setup (optional)

For information on how to configure the example for an optional Microsoft Azure\*, IBM Bluemix\*, AT&T M2X\*, AWS\*, Predix\*, or SAP\* IoT cloud server, go to:

[https://github.com/intel-iot-devkit/iot-samples-cloud-setup](https://github.com/intel-iot-devkit/iot-samples-cloud-setup)

IMPORTANT NOTICE: This software is sample software. It is not designed or intended for use in any medical, life-saving or life-sustaining systems, transportation systems, nuclear systems, or for any other mission-critical application in which the failure of the system could lead to critical injury or death. The software may not be fully tested and may contain bugs or errors; it may not be intended or suitable for commercial release. No regulatory approvals for the software have been obtained, and therefore software may not be certified for use in certain countries or environments.
