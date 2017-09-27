# Equipment Activity Monitor

## Introduction

This shop-floor equipment activity monitor application is part of a series of how-to Internet of Things (IoT) code sample exercises using the Intel® IoT Developer Kit and a compatible Intel-based platform, cloud platforms, APIs, and other technologies.

From this exercise, developers will learn how to:<br>

- Interface with and sensors using MRAA and UPM from the Intel® IoT Developer Kit, a complete hardware and software solution to help developers explore the IoT and implement innovative projects.<br>
- Set up a web application server to let users enter the access code to disable the alarm system and store this alarm data using Azure Redis Cache\* from Microsoft Azure\*, Redis Store\* from IBM Bluemix\*, or Elasticache\* using Redis\* from Amazon Web Services (AWS)\*, different cloud services for connecting IoT solutions including data analysis, machine learning, and a variety of productivity tools to simplify the process of connecting your sensors to the cloud and getting your IoT project up and running quickly.
- Connect to a server using IoT Hub from Microsoft Azure\*, IoT from IBM Bluemix\*, IoT from Amazon Web Services (AWS)\*, AT&T M2X\*, Predix\* from GE, or SAP Cloud Platform\* IoT, different cloud-based IoT platforms for machine to machine communication.

Available in:  
 ![C++](./cpp)  
 ![Java\*](./java)  
 ![JavaScript\*](./javascript)  
 ![Python\*](./python) 

## What it is

Using a compatible Intel-based platform, this project lets you create a shop-floor equipment activity monitor that:<br>
- tracks equipment usage by monitoring sound and vibration sensors.<br>
- issues a visual notification whenever the equipment is in use.<br>
- logs equipment usage using cloud-based data storage.

## How it works

This equipment activity monitor watches for sound and vibration.

If both exceed a defined threshold, it lights up the display to indicate the equipment is in use.

Once the equipment is no longer used, it clears the display.

Optionally, data can be stored using your own Microsoft Azure\*, IBM Bluemix\*, AT&T M2X\*, AWS\*, Predix\*, or SAP\* account.

## First time setup  
For all the samples in this repository, see the ![General Setup Instructions](./../README.md#setup) for required boards and libraries.

## Hardware requirements

In addition to using a compatible platform listed in ![Hardware Compatibility](./../README.md#hardware-compatibility), here is additional hardware you will need to run this example.

This sample can be used with either Grove or DFRobot components.

1. [Grove\* Base Shield V2](https://www.seeedstudio.com/Base-Shield-V2-p-1378.html)
2. [Grove\* Sound Sensor](http://www.seeedstudio.com/depot/Grove-Sound-Sensor-p-752.html)
3. [Grove\* Piezo Vibration Sensor](http://www.seeedstudio.com/depot/Grove-Piezo-Vibration-Sensor-p-1411.html)
4. [Grove\* RGB LCD](http://iotdk.intel.com/docs/master/upm/node/classes/jhd1313m1.html)

DFRobot\* Starter Kit for Intel® Edison, containing:

1. [Analog Sound Sensor](http://www.dfrobot.com/index.php?route=product/product&product_id=83).
3. [Digital Vibration Sensor](http://www.dfrobot.com/index.php?route=product/product&product_id=79)
4. [LCD Keypad Shield](http://iotdk.intel.com/docs/master/upm/node/classes/sainsmartks.html)

### Connecting the Grove\* sensors

![](./images/equipment-activity-grove.JPG)

You need to have a Grove\* Shield connected to an Arduino\* compatible breakout board to plug all the Grove\* devices into the Grove\* Shield. Make sure you have the tiny VCC switch on the Grove\* Shield set to **5V**.

Sensor | Pin
--- | ---
Grove\* Sound Sensor | A0
Grove\* Piezo Vibration Sensor | A2
Grove\* RGB LCD | I2C

### Connecting the DFRobot\* sensors

![](./images/equipment-activity-dfrobot.JPG)

You need to have a LCD Keypad Shield connected to an Arduino\* compatible breakout board to plug all the DFRobot\* devices into the LCD Keypad Shield.

Sensor | Pin
--- | ---
Analog Sound Sensor | A1
Digital Vibration Sensor | A2

### Determining your platform's IP Address

See the section on Finding Your Board's IP Address at the bottom of the ![Setup Instructions](./../README.md#finding-your-boards-ip-address) readme. 

IMPORTANT NOTICE: This software is sample software. It is not designed or intended for use in any medical, life-saving or life-sustaining systems, transportation systems, nuclear systems, or for any other mission-critical application in which the failure of the system could lead to critical injury or death. The software may not be fully tested and may contain bugs or errors; it may not be intended or suitable for commercial release. No regulatory approvals for the software have been obtained, and therefore software may not be certified for use in certain countries or environments.
