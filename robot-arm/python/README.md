# Robot Arm in Python*

## What it is

Using a compatible Intel-based platform, this project lets you create a robot arm that:<br>
- continuously checks the Grove\* Joystick.<br>
- moves two stepper motors based on the joystick control.<br>
- can be accessed via the built-in web interface to control the motors.

## First time setup
For all the samples in this repository, see the ![General Setup Instructions](./../../README.md#setup) for required boards and libraries.  You need either Grove\* or DFRobot\* sensors but not both.

## Hardware requirements

### Grove\*

Sensor | Pin
--- | ---
Stepper motor controller #1 | Digital pins 4, 5, 6, and 7
Stepper motor controller #2 | Digital pins 9, 10, 11, and 12
Both controllers to ground (GND) | 5V power coming from the Arduino\* breakout board (VCC), and to the separate 5V power for the motors (VM).

For more specific information on the hardware requirements see ![Hardware Details](./../README.md#hardware-requirements)

## Software requirements

1. ![MRAA](https://github.com/intel-iot-devkit/mraa) and ![UPM](https://upm.mraa.io/) 
2. Python 2.7
3. Packages as listed in the ![setup.py](setup.py) file in this directory

You can install the dependencies.

## Python Package Manager (pip)

To install the Python\* package manager needed to install and run the example:

    $ pip install --upgrade pip setuptools

## Install the example

Once all dependencies are installed you can install the example itself with the following command:

    $ pip install --src ~/python/examples/ -e "git+https://github.com/intel-iot-devkit/how-to-code-samples.git#egg=iot_robot_arm&subdirectory=robot-arm/python"

The `pip` command will install required Python dependencies, save the source code for the example in `~/python/examples/iot_robot_arm/` and link the package to the global Python `site-packages` folder.

## Configuring the example

When the example is installed through `pip` the `config.json` file that holds the configuration for the example lives in `~/python/examples/iot_robot_arm/robot-arm/python/iot_robot_arm/config.json`.

To configure the example for the Grove\* kit, just leave the `kit` key in the `config.json` set to `grove`.

To configure the example for the Arduino 101, add a `PLATFORM` key with the value `firmata` to the `config.json`, as follows:

```JSON
{
  "kit": "grove",
  "PLATFORM": "firmata"
}
```

## Running the program 

Once the example is installed through `pip` you can run the program by running the following command in an SSH session to the board:

    $ python -m iot_robot_arm

You will see output similar to below when the program is running.

```
move 2 1020 
move 1 1020 
move 2 1020 
```

Refer to ![How it Works](./../README.md#how-it-works) for details on the functionality.

IMPORTANT NOTICE: This software is sample software. It is not designed or intended for use in any medical, life-saving or life-sustaining systems, transportation systems, nuclear systems, or for any other mission-critical application in which the failure of the system could lead to critical injury or death. The software may not be fully tested and may contain bugs or errors; it may not be intended or suitable for commercial release. No regulatory approvals for the software have been obtained, and therefore software may not be certified for use in certain countries or environments.
