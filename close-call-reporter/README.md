# Close Call Reporter

## Introduction

This close call fleet driving reporter application is part of a series of how-to Internet of Things (IoT)code sample exercises using the Intel® IoT Developer Kit and a compatible Intel® IoT Platform, cloud platforms, APIs, and other technologies.

From this exercise, developers will learn how to:<br>
- Interface with and sensors using MRAA and UPM from the Intel® IoT Developer Kit, a complete hardware and software solution to help developers explore the IoT and implement innovative projects.<br>
- Store the close-call data using Azure Redis Cache\* from Microsoft Azure\*, Redis Store\* from IBM Bluemix\*, or Elasticache\* using Redis\* from Amazon Web Services (AWS)\*, different cloud services for connecting IoT solutions including data analysis, machine learning, and a variety of productivity tools to simplify the process of connecting your sensors to the cloud and getting your IoT project up and running quickly.
- Connect to a server using IoT Hub from Microsoft Azure\*, IoT from IBM Bluemix\*, IoT from Amazon Web Services (AWS)\*, AT&T M2X\*, Predix\* from GE, or SAP Cloud Platform\* IoT, different cloud-based IoT platforms for machine to machine communication.

Available in:  
 ![C++](./cpp)  
 ![Java\*](./java)  
 ![JavaScript\*](./javascript)  
 ![Python\*](./python)  

## What it is

Using a compatible Intel® IoT Platform, this project lets you create a close call fleet driving reporter that:<br>
- monitors the Grove\* IR Distance Interrupter.<br>
- monitors the Grove\* GPS.<br>
- keeps track of close calls and logs them using cloud-based data storage.

## How it works

This close call reporter system monitors the direction the IR Distance sensor is pointed to.

It also keeps track of the GPS position, updating the position frequently to ensure accurate data.

Optionally, data can be stored using your own Microsoft Azure\*, IBM Bluemix\*, AT&T M2X\*, AWS\*, Predix\*, or SAP\* account.

## First time setup  
For all the samples in this repository, see the ![General Setup Instructions](./README.md#setup) for required boards and libraries.

## Hardware requirements
In addition to using a compatible platform listed in ![Hardware Compatibility](./README.md#hardware-compatibility), here is additional hardware you will need to run this example

This sample can be used with either Grove\* or DFRobot\* components.

Grove\* Transportation and Safety Kit, containing:

1. [Grove\* Base Shield V2](https://www.seeedstudio.com/Base-Shield-V2-p-1378.html)
2. [Grove\* IR Distance Interrupter](http://iotdk.intel.com/docs/master/upm/node/classes/rfr359f.html)
3. [Grove\* GPS](http://iotdk.intel.com/docs/master/upm/node/classes/ublox6.html)

DFRobot\* Starter Kit for Intel® Edison, containing:

1. [IR Distance Sensor](http://www.dfrobot.com/index.php?route=product/product&product_id=572)
3. [GPS](http://iotdk.intel.com/docs/master/upm/node/classes/ublox6.html)
4. [I/O Expansion Shield](http://www.dfrobot.com/index.php?route=product/product&product_id=1009)

### Connecting the Grove\* sensors

![](./images/close-call-grove.JPG)

You need to have a Grove\* Shield connected to an Arduino\* compatible breakout board to plug all the Grove devices into the Grove Shield. Make sure you have the tiny VCC switch on the Grove\* Shield set to **5V**.

Sensor | Pin
--- | ---
Grove\* IR Distance Interrupter | D2
Grove\* GPS | UART port

### Connecting the DFRobot\* sensors

![](./images/close-call-dfrobot.JPG)

You need to have a DFRobot\* I/O Expansion Shield connected to an Arduino\* compatible breakout board to plug all the DFRobot\* devices into the DFRobot\* I/O Expansion Shield.

Sensor | Pin
--- | ---
IR Distance Sensor | D4
GPS cable TX (white) | RX pin
GPS cable RX (black) | TX pin
GPS cable power (red) | 5V pins
GPS cable ground (thicker black) | GND

### Determining your platform's IP address

See the section on Finding Your Board's IP Address at the bottom of the ![Setup Instructions](./../../README.md#finding-your-boards-ip-address) readme. 

IMPORTANT NOTICE: This software is sample software. It is not designed or intended for use in any medical, life-saving or life-sustaining systems, transportation systems, nuclear systems, or for any other mission-critical application in which the failure of the system could lead to critical injury or death. The software may not be fully tested and may contain bugs or errors; it may not be intended or suitable for commercial release. No regulatory approvals for the software have been obtained, and therefore software may not be certified for use in certain countries or environments.
