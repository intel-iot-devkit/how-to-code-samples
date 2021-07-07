# Robot arm in C++

## What it is

Using a compatible Intel-based platform, this project lets you create a robot arm that:

- continuously checks the Grove\* Joystick.
- moves two stepper motors, based on the control of the joystick.
- can be accessed via a built-in web interface to control the motors.

## First time setup
For all the samples in this repository, see the [General Setup Instructions](./../../README.md#setup) for required boards and libraries.  You need either Grove\* or DFRobot\* sensors but not both.

## Hardware requirements

### Grove\* 

Sensor | Pin
--- | ---
Stepper motor controller #1 | Digital pins 4, 5, 6, and 7
Stepper motor controller #2 | Digital pins 9, 10, 11, and 12
Both controllers to ground (GND) | 5V power coming from the Arduino\* breakout board (VCC), and to the separate 5V power for the motors (VM).

For more specific information on the hardware requirements see ![Hardware Details](./../README.md#hardware-requirements)

## Software requirements

1. [MRAA](https://github.com/intel-iot-devkit/mraa) and [UPM](https://upm.mraa.io)
Note: The following libraries are included with the repo and already linked to in the code -jsoncpp -restclient
2. Intel® System Studio [https://software.intel.com/en-us/creating-iot-projects-with-intel-system-studio-2018-c](https://software.intel.com/en-us/creating-iot-projects-with-intel-system-studio-2018-c) 


## Expected output

```
move 2 1020 
move 1 1020 
move 2 1020 
```
### Controlling the robot arm via a web browser

The robot arm is controlled using a single-page web interface served directly from your board while the sample program is running.<br>
[](./../../images/cpp/robot-arm-web.png)

The web server runs on port `3000`; if your board is connected to Wi-Fi on `192.168.1.13`, the address to browse to if you are on the same network is `http://192.168.1.13:3000`.

## Regenerating HTML and CSS

If you make any changes to either the **index.html** or **styles.css** file, you need to regenerate the .hex file used to serve up the assets via the built-in Crow\* web server.

For help using the shell script, go to this link:

[how-to-run-the-shellscript.md](./../../docs/cpp/how-to-run-the-shellscript.md)

IMPORTANT NOTICE: This software is sample software. It is not designed or intended for use in any medical, life-saving or life-sustaining systems, transportation systems, nuclear systems, or for any other mission-critical application in which the failure of the system could lead to critical injury or death. The software may not be fully tested and may contain bugs or errors; it may not be intended or suitable for commercial release. No regulatory approvals for the software have been obtained, and therefore software may not be certified for use in certain countries or environments.
