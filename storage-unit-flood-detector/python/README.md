# Storage Unit Flood Detector in Python*

## What it is

Using a compatible Intel-based platform, this project lets you create a smart access control system that:
- continuously checks the moisture sensor.
- sounds an audible warning in case of a possible flooding.
- stores a record of each time water is detected, using cloud-based data storage.

## First time setup
For all the samples in this repository, see the ![General Setup Instructions](./../../README.md#setup) for required boards and libraries.  You need either Grove\* or DFRobot\* sensors but not both.

## Hardware requirements

### Grove\* 

Sensor | Pin
--- | ---
Grove\* Moisture Sensor | A0
Grove\* Speaker | D5

### DFRobot\* 

Sensor | Pin
--- | ---
Moisture Sensor | A3
Buzzer | A1

For more specific information on the hardware requirements see ![Hardware Details](./../README.md#hardware-requirements)

## Software requirements

1. [MRAA](https://github.com/intel-iot-devkit/mraa) and [UPM](https://upm.mraa.io/) 
2. Python 2.7
3. Packages as listed in the [setup.py](setup.py) file in this directory
1. Microsoft Azure\*, IBM Bluemix\*, AT&T M2X\*, AWS\*, Predix\*, or SAP\* account (optional)

You can install the dependencies.

## Python\* Package Manager (pip)

To install the Python\* package manager needed to install and run the example:

    $ pip install --upgrade pip setuptools

## Install the example

Once all dependencies are installed you can install the example itself with the following command:

    $ pip install --src ~/python/examples/ -e "git+https://github.com/intel-iot-devkit/how-to-code-samples.git#egg=iot_storage_unit_flood_detector&subdirectory=storage-unit-flood-detector/python"

The `pip` command will install required Python dependencies, save the source code for the example in `~/python/examples/iot_storage_unit_flood_detector/` and link the package to the global Python `site-packages` folder.

## Configuring the example

When the example is installed through `pip` the `config.json` file that holds the configuration for the example lives in `~/python/examples/iot_storage_unit_flood_detector/storage-unit-flood-detector/python/iot_storage_unit_flood_detector/config.json`.

To configure the example for the Grove\* kit, just leave the `kit` key in the `config.json` set to `grove`. To configure the example for the DFRobot\* kit, change the `kit` key in the `config.json` to `dfrobot` as follows:

```JSON
{
  "kit": "dfrobot"
}
```

To configure the example for the Arduino 101, add a `PLATFORM` key with the value `firmata` to the `config.json`, as follows:

```JSON
{
  "kit": "grove",
  "PLATFORM": "firmata"
}
```

To configure the example for the optional Microsoft Azure\*, IBM Bluemix\*, or AWS\* data store, add the `SERVER` and `AUTH_TOKEN` keys in the `config.json` file below the "CODE" key as follows:

```JSON
{
  "kit": "grove",
  "SERVER": "http://intel-examples.azurewebsites.net/logger/access-control",
  "AUTH_TOKEN": "s3cr3t"
}
```

For information on how to configure the example for an optional Microsoft Azure\*, IBM Bluemix\*, AT&T M2X\*, AWS\*, Predix\*, or SAP\* IoT cloud server, go to:

[https://github.com/intel-iot-devkit/iot-samples-cloud-setup](https://github.com/intel-iot-devkit/iot-samples-cloud-setup)

## Running the program manually

Once the example is installed through `pip` you can run the program by running the following command in an SSH session to the board:

    $ python -m iot_storage_unit_flood_detector
    
You will see output similar to below when the program is running.

```
UPLOADING: Uploading project bundle to IoT device. 
[Upload Complete] Moisture alarm 
Connecting to MQTT server... 
MQTT message published: { d: { value: '2016-04-22705:08:41.682Z' } } 
```

Refer to [How it Works](./../README.md#how-it-works) for details on the functionality.

IMPORTANT NOTICE: This software is sample software. It is not designed or intended for use in any medical, life-saving or life-sustaining systems, transportation systems, nuclear systems, or for any other mission-critical application in which the failure of the system could lead to critical injury or death. The software may not be fully tested and may contain bugs or errors; it may not be intended or suitable for commercial release. No regulatory approvals for the software have been obtained, and therefore software may not be certified for use in certain countries or environments.
