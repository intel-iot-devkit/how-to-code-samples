# Access control in Python*

## What it is

Using a compatible Intel-based platform, this project lets you create a smart access control system that:<br>
- monitors a motion sensor to detect when a person is in an area that requires authorization.<br>
- can be accessed with your mobile phone via the built-in web interface to disable the alarm.<br>
- keeps track of access, using cloud-based data storage.

## First time setup
For all the samples in this repository, see the ![General Setup Instructions](./../../README.md#setup) for required boards and libraries.  You need either Grove\* or DFRobot\* sensors but not both.

## Hardware requirements

### Grove\*

Sensor | Pin
--- | ---
PIR Motion Sensor | D4
Grove RGB LCD | I2C

### DFRobot\* 

Sensor | Pin
--- | ---
PIR (Motion) Sensor | A2

For more specific information on the hardware requirements see ![Hardware Details](./../README.md#hardware-requirements)

## Software requirements

1. ![MRAA](https://github.com/intel-iot-devkit/mraa) and ![UPM](https://upm.mraa.io/) 
2. Python 2.7
3. Packages as listed in the ![setup.py](setup.py) file in this directory
1. Microsoft Azure\*, IBM Bluemix\*, AT&T M2X\*, AWS\*, Predix\*, or SAP\* account (optional)

You can install the dependencies.

## Configuring the example

When the example is installed through `pip` the `config.json` file that holds the configuration for the example lives in `~/python/examples/iot_access_control/access-control/python/iot_access_control/config.json`.

To configure the example for the Grove\* kit, just leave the `kit` key in the `config.json` set to `grove`. To configure the example for the DFRobot\* kit, change the `kit` key in the `config.json` to `dfrobot` as follows:

```JSON
{
  "kit": "dfrobot",
  "CODE": "4321"
}
```

To configure the example for the Arduino 101, add a `PLATFORM` key with the value `firmata` to the `config.json`, as follows:

```JSON
{
  "kit": "dfrobot",
  "CODE": "4321",
  "PLATFORM": "firmata"
}
```

To configure the example for the optional Microsoft Azure\*, IBM Bluemix\*, or AWS\* data store, add the `SERVER` and `AUTH_TOKEN` keys in the `config.json` file below the "CODE" key as follows:

```JSON
{
  "kit": "grove",
  "CODE": "4321",
  "SERVER": "http://intel-examples.azurewebsites.net/logger/access-control",
  "AUTH_TOKEN": "s3cr3t"
}
```

For information on how to configure the example for an optional Microsoft Azure\*, IBM Bluemix\*, AT&T M2X\*, AWS\*, Predix\*, or SAP\* IoT cloud server, go to:

[https://github.com/intel-iot-devkit/iot-samples-cloud-setup](https://github.com/intel-iot-devkit/iot-samples-cloud-setup)

## Running the program

Once the program is installed and configured as detailed above, you can execute the program by running the following command in an SSH session to the board:

    $ python -m iot_access_control

You will see output similar to below when the program is running.
 
```
MQTT message published: { d: { value: '2016-04-21T23:57:18.254Z looking-for-motion'  }}
2016-04-21T23:57:30.559Z motion-detected 
Connecting to MQTT server... 
MQTT message published: { d: { value: '2016-04-21T23:57:30.559Z motion-detected'  }}
```

IMPORTANT NOTICE: This software is sample software. It is not designed or intended for use in any medical, life-saving or life-sustaining systems, transportation systems, nuclear systems, or for any other mission-critical application in which the failure of the system could lead to critical injury or death. The software may not be fully tested and may contain bugs or errors; it may not be intended or suitable for commercial release. No regulatory approvals for the software have been obtained, and therefore software may not be certified for use in certain countries or environments.
