# Earthquake Detector in Python*

## What it is

Using a compatible Intel-based platform, this project lets you create an earthquake detector that:<br>
- senses motion using the digital accelerometer.<br>
- checks live earthquake data, using the USGS\* API.<br>
- displays the earthquake on the LCD.

## First time setup
For all the samples in this repository, see the [General Setup Instructions](./../../README.md#setup) for required boards and libraries.  You need either Grove\* or DFRobot\* sensors but not both.

## Hardware requirements

### Grove\*

Sensor | Pin
--- | ---
Grove\* 3-Axis Digital Accelerometer | I2C
Grove\* RGB LCD | I2C

### DFRobot\* 

Sensor | Pin
--- | ---
One end of a DFRobot\* cable into the plug labeled "X" on the Triple-Axis Accelerometer | A1
One end of a DFRobot\* cable into the plug labeled "Y" on the Triple-Axis Accelerometer | A2
One end of a DFRobot\* cable into the plug labeled "Z" on the Triple-Axis Accelerometer | A3

For more specific information on the hardware requirements see [Hardware Details](./../README.md#hardware-requirements)

## Software requirements

1. [MRAA](https://github.com/intel-iot-devkit/mraa) and [UPM](https://upm.mraa.io/) 
2. Python 2.7
3. Packages as listed in the [setup.py](setup.py) file in this directory

You can install the dependencies.

## Python\* Package Manager (pip)

To install the Python\* package manager needed to install and run the example:

    $ pip install --upgrade pip setuptools

## Install the example

Once all dependencies are installed you can install the example itself with the following command:

    $ pip install --src ~/python/examples/ -e "git+https://github.com/intel-iot-devkit/how-to-code-samples.git#egg=iot_earthquake_detector&subdirectory=earthquake-detector/python"

The `pip` command will install required Python dependencies, save the source code for the example in `~/python/examples/iot_earthquake_detector/` and link the package to the global Python `site-packages` folder.

## Configuring the example

When the example is installed through `pip` the `config.json` file that holds the configuration for the example lives in `~/python/examples/iot_earthquake_detector/earthquake-detector/python/iot_earthquake_detector/config.json`.

To configure the example for the Grove\* kit, just leave the `kit` key in the `config.json` set to `grove`. To configure the example for the DFRobot\* kit, change the `kit` key in the `config.json` to `dfrobot` as follows:

```JSON
{
    "kit": "dfrobot",
    "LATITUDE": "47.641944",
    "LONGITUDE": "-122.127222"
}
```

To configure the example for the Arduino 101, add a `PLATFORM` key with the value `firmata` to the `config.json`, as follows:

```JSON
{
  "kit": "dfrobot",
  "PLATFORM": "firmata"
}
```

## Running the program

Once the program is installed and configured as detailed above, you can execute the program by running the following command in an SSH session to the board:

    $ python -m iot_earthquake_detector

You will see output similar to below when the program is running.

```
Checking... 
No quake. 
```

Refer to [How it Works](./../README.md#how-it-works) for details on the functionality.

IMPORTANT NOTICE: This software is sample software. It is not designed or intended for use in any medical, life-saving or life-sustaining systems, transportation systems, nuclear systems, or for any other mission-critical application in which the failure of the system could lead to critical injury or death. The software may not be fully tested and may contain bugs or errors; it may not be intended or suitable for commercial release. No regulatory approvals for the software have been obtained, and therefore software may not be certified for use in certain countries or environments.
