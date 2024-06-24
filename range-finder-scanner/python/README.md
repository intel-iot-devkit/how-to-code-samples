# Range Finder Scanner in Python*

## What it is

Using a compatible Intel-based platform, this project lets you create a range finding scanner that:<br>
- continuously checks the Grove\* IR Distance Interrupter.<br>
- moves the stepper motor in a 360-degree circle.<br>
- can be accessed via the built-in web interface to view range finder data.

## First time setup
For all the samples in this repository, see the ![General Setup Instructions](./../../README.md#setup) for required boards and libraries.  You need either Grove\* or DFRobot\* sensors but not both.

## Hardware requirements

### Grove\* Robotics Kit containing:

Sensor | Pin
--- | ---
Stepper motor controller | Digital pins 9, 10, 11, and 12
Controller to ground (GND) | 5V (VCC) and 5V (VM)

For more specific information on the hardware requirements see ![Hardware Details](./../README.md#hardware-requirements)

## Software requirements

1. [MRAA](https://github.com/intel-iot-devkit/mraa) and [UPM](https://upm.mraa.io/) 
2. Python 2.7
3. Packages as listed in the [setup.py](setup.py) file in this directory

You can install the dependencies.

## Configuring the example

When the example is installed through `pip` the `config.json` file that holds the configuration for the example lives in `~/python/examples/iot_range_finder_scanner/range-finder-scanner/python/iot_range_finder_scanner/config.json`.

To configure the example for the Arduino 101\*, add a `PLATFORM` key with the value `firmata` to the `config.json`, as follows:

```JSON
{
  "kit": "grove",
  "PLATFORM": "firmata"
}
```

## Running the program

Once the example is installed through `pip` you can run the program by running the following command in an SSH session to the board:

    $ python -m iot_range_finder_scanner
    
You will see output similar to below when the program is running.

```
296 false
297 false
298 false
299 false 
```

Refer to ![How it Works](./../README.md#how-it-works) for details on the functionality.

IMPORTANT NOTICE: This software is sample software. It is not designed or intended for use in any medical, life-saving or life-sustaining systems, transportation systems, nuclear systems, or for any other mission-critical application in which the failure of the system could lead to critical injury or death. The software may not be fully tested and may contain bugs or errors; it may not be intended or suitable for commercial release. No regulatory approvals for the software have been obtained, and therefore software may not be certified for use in certain countries or environments.
