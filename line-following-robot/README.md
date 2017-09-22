# Line Following Robot

## Introduction

This line following robot application is part of a series of how-to Internet of Things (IoT) code sample exercises using the Intel® IoT Developer Kit and a compatible Intel® IoT Platform, cloud platforms, APIs, and other technologies.

From this exercise, developers will learn how to:<br>
- Interface with and sensors using MRAA and UPM from the Intel® IoT Developer Kit, a complete hardware and software solution to help developers explore the IoT and implement innovative projects.<br>
- Store line detection data using Azure Redis Cache\* from Microsoft Azure\*, Redis Store\* from IBM Bluemix\*, or Elasticache\* using Redis\* from Amazon Web Services (AWS)\*, different cloud services for connecting IoT solutions including data analysis, machine learning, and a variety of productivity tools to simplify the process of connecting your sensors to the cloud and getting your IoT project up and running quickly.
- Connect to a server using IoT Hub from Microsoft Azure\*, IoT from IBM Bluemix\*, IoT from Amazon Web Services (AWS)\*, AT&T M2X\*, Predix\* from GE, or SAP Cloud Platform\* IoT, different cloud-based IoT platforms for machine to machine communication.

Available in:  
 ![C++](./cpp)  
 ![Java\*](./java)  
 ![JavaScript\*](./javascript)  
 ![Python\*](./python) 

## What it is

Using a compatible Intel® IoT Platform, this project lets you create a line following robot that:<br>
- continuously checks the line finder sensor.<br>
- moves forward if on the line, using the stepper motors.<br>
- tries to pivot to find the line if not on it, using the stepper motors.<br>
- logs events from the line finder sensor, using cloud-based data storage.

## How it works

The line finder robot uses the two attached motors to attempt to follow a line, keeping track of it with the line finder sensor. If on the line, it moves forward. Otherwise, it pivots in place, trying to locate it using the line finder sensor.

Optionally, data can be stored using your own Microsoft Azure\*, IBM Bluemix\*, AT&T M2X\*, AWS\*, Predix\*, or SAP\* account.

## First time setup  
For all the samples in this repository, see the ![General Setup Instructions](./../README.md#setup) for required boards and libraries.

## Hardware requirements

In addition to using a compatible platform listed in ![Hardware Compatibility](./../README.md#hardware-compatibility), here is additional hardware you will need to run this example.

Grove\* Robotics Kit containing:

1. [Grove\* Base Shield V2](https://www.seeedstudio.com/Base-Shield-V2-p-1378.html)
2. [Grove Line Finder](http://iotdk.Intel®.com/docs/master/upm/node/classes/grovelinefinder.html)
3. [Stepper Motor Controller & Stepper Motor](http://iotdk.Intel®.com/docs/master/upm/node/classes/uln200xa.html) (x2)

### Connecting the Grove\* sensors

![](./../images/line-follower-grove.JPG)

You need to have a Grove\* Shield connected to an Arduino\* compatible breakout board to plug all the Grove devices into the Grove Shield. Make sure you have the tiny VCC switch on the Grove Shield set to **5V**.

You need to power the Intel® IoT Platform with the external power adapter that comes with your starter kit, or substitute it with an external 12V 1.5A power supply. You can also use an external battery, such as a 5V USB battery.

In addition, you need a breadboard and an extra 5V power supply to provide power to both motors. Note: you need a separate battery or power supply for the motors. You cannot use the same power supply for both the Intel® IoT Platform and the motors, so you need either 2 batteries or 2 power supplies in total.

Sensor | Pin
--- | ---
Stepper motor controller #1 | Digital pins 4, 5, 6, and 7
Stepper motor controller #2 | Digital pins 9, 10, 11, and 12
Both controllers to Ground (GND) | 5V power (VCC), and 5V power (VM)
Grove\* Line Finder | D2

Look at the bottom of the shield to determine the pin numbers.

### Determining your platform's IP Address

See the section on Finding Your Board's IP Address at the bottom of the ![Setup Instructions](./../README.md#finding-your-boards-ip-address) readme. 

IMPORTANT NOTICE: This software is sample software. It is not designed or intended for use in any medical, life-saving or life-sustaining systems, transportation systems, nuclear systems, or for any other mission-critical application in which the failure of the system could lead to critical injury or death. The software may not be fully tested and may contain bugs or errors; it may not be intended or suitable for commercial release. No regulatory approvals for the software have been obtained, and therefore software may not be certified for use in certain countries or environments.
