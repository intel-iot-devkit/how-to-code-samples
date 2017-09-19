# Robot Arm

## Introduction

This robot arm application is part of a series of how-to Internet of Things (IoT) code sample exercises using the Intel® IoT Developer Kit and a compatible Intel® IoT Platform, cloud platforms, APIs, and other technologies.

From this exercise, developers will learn how to:<br>
- Interface with and sensors using MRAA and UPM from the Intel® IoT Developer Kit, a complete hardware and software solution to help developers explore the IoT and implement innovative projects.<br>
- Set up a web application server to control a robot arm using a web page served directly from the Intel® IoT Platform.

Available in:  
 ![C](./cpp)  
 ![Java\*](./java)  
 ![JavaScript\*](./javascript)  
 ![Python\*](./python) 

## What it is

Using a compatible Intel® IoT Platform, this project lets you create a robot arm that:<br>
- continuously checks the Grove\* Joystick.<br>
- moves two stepper motors based on the joystick control.<br>
- can be accessed via the built-in web interface to control the motors.

## How it works

The robot arm example allows you to control a robotic arm using a thumb joystick.
Each axis of the joystick corresponds to a motor to control.

## First time setup  
For all the samples in this repository, see the ![General Setup Instructions](./../README.md#setup) for required boards and libraries.

## Hardware requirements

In addition to using a compatible platform listed in ![Hardware Compatibility](./../README.md#hardware-compatibility), here is additional hardware you will need to run this example.

Grove\* Robotics Kit containing:

1. [Grove\* Base Shield V2](https://www.seeedstudio.com/Base-Shield-V2-p-1378.html)
2. [Grove Thumb Joystick](http://iotdk.intel.com/docs/master/upm/node/classes/joystick12.html)
3. [Stepper Motor Controller & Stepper Motor](http://iotdk.intel.com/docs/master/upm/node/classes/uln200xa.html) (x2)

### Connecting the Grove\* sensors

![](./robot-arm-grove.JPG)

You need to have a Grove\* Shield connected to an Arduino\* compatible breakout board to plug all the Grove devices into the Grove Shield. Make sure you have the tiny VCC switch on the Grove Shield set to **5V**.

You need to power the Intel® IoT Platform with the external power adapter that comes with your starter kit, or substitute it with an external 12V 1.5A power supply. You can also use an external battery, such as a 5V USB battery.

In addition, you need a breadboard and an extra 5V power supply to provide power to both motors. Note: you need a separate battery or power supply for the motors. You cannot use the same power supply for both the Intel® IoT Platform and the motors, so you need either 2 batteries or 2 power supplies in total.

Sensor | Pin
--- | ---
Stepper motor controller #1 | Digital pins 4, 5, 6, and 7
Stepper motor controller #2 | Digital pins 9, 10, 11, and 12
Both controllers to ground (GND) | 5V power coming from the Arduino\* breakout board (VCC), and to the separate 5V power for the motors (VM).

Look at the bottom of the shield to determine the pin numbers.

2. Plug one end of a Grove\* cable into the Grove\* Thumb Joystick, and connect the other end to the A2 port on the Grove\* Shield.

### Controlling via a browser

![](./../../images/js/robot-arm-web.png)

Optionally, the motors can be controlled directly via a web page served by the program running on the Intel® IoT Platform.

The web server runs on port `3000`, so if the Intel® IoT Platform is connected to Wi-Fi on `192.168.1.13`, the address to browse to if you are on the same network is `http://192.168.1.13:3000`.

### Determining your platform's IP Address

See the section on Finding Your Board's IP Address at the bottom of the ![Setup Instructions](./../README.md#finding-your-boards-ip-address) readme. 

IMPORTANT NOTICE: This software is sample software. It is not designed or intended for use in any medical, life-saving or life-sustaining systems, transportation systems, nuclear systems, or for any other mission-critical application in which the failure of the system could lead to critical injury or death. The software may not be fully tested and may contain bugs or errors; it may not be intended or suitable for commercial release. No regulatory approvals for the software have been obtained, and therefore software may not be certified for use in certain countries or environments.
