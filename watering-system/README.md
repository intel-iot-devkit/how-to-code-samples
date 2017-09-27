# Watering System

## Introduction

This automatic watering system application is part of a series of how-to Internet of Things (IoT) code sample exercises using the Intel® IoT Developer Kit and a compatible Intel-based platform, cloud platforms, APIs, and other technologies.

From this exercise, developers will learn how to:<br>

- Interface with and sensors using MRAA and UPM from the Intel® IoT Developer Kit, a complete hardware and software solution to help developers explore the IoT and implement innovative projects.<br>
- Set up a web application server to let users enter the access code to disable the alarm system and store this alarm data using Azure Redis Cache\* from Microsoft Azure\*, Redis Store\* from IBM Bluemix\*, or Elasticache\* using Redis\* from Amazon Web Services (AWS)\*, different cloud services for connecting IoT solutions including data analysis, machine learning, and a variety of productivity tools to simplify the process of connecting your sensors to the cloud and getting your IoT project up and running quickly.
- Connect to a server using IoT Hub from Microsoft Azure\*, IoT from IBM Bluemix\*, IoT from Amazon Web Services (AWS)\*, AT&T M2X\*, Predix\* from GE, or SAP Cloud Platform\* IoT, different cloud-based IoT platforms for machine to machine communication.
- Invoke the services of the Twilio\* API for sending text messages.

Available in:  
 ![C++](./cpp)  
 ![Java\*](./java)  
 ![JavaScript\*](./javascript)  
 ![Python\*](./python) 

## What it is

Using a compatible Intel-based platform, this project lets you create an automatic watering system that:<br>
- turns a water pump on or off based on a configurable schedule.<br>
- detects if the pumping occurs when expected, by using a water flow sensor.<br>
- can be accessed with your mobile phone via the built-in web interface to set the watering times;<br>
- keeps track of watering events, using cloud-based data storage.<br>
- sends text messages to alert recipients if the system is not working as expected.

## How it works

This watering system allows you to set the watering schedule via a web page served directly from an Intel-based platform, by using your mobile phone.

It automatically checks moisture sensor data at periodic intervals, and displays this data on the web page.

If the water pump is supposed to be on but the water flow sensor does not detect that the pumping is talking place as expected, it sends a text message to a specified number through Twilio\* so the watering system can be repaired.

Optionally, data can be stored using your own Microsoft Azure\*, IBM Bluemix\*, AT&T M2X\*, AWS\*, Predix\*, or SAP\* account.

## First time setup  
For all the samples in this repository, see the ![General Setup Instructions](./../README.md#setup) for required boards and libraries.

## Hardware requirements

In addition to using a compatible platform listed in ![Hardware Compatibility](./../README.md#hardware-compatibility), here is additional hardware you will need to run this example

This sample can be used with either Grove\* or DFRobot\* components.

Grove\* Environment & Agriculture Kit, containing:

1. [Grove\* Base Shield V2](https://www.seeedstudio.com/Base-Shield-V2-p-1378.html)
2. [Grove\* Moisture Sensor](http://iotdk.intel.com/docs/master/upm/node/classes/grovemoisture.html)
3. [Water Pump](http://www.seeedstudio.com/depot/6V-Mini-Water-Pump-p-1945.html)
4. [Water Flow Sensor](http://www.seeedstudio.com/depot/G18-Water-Flow-Sensor-p-1346.html)
5. [Grove\* Dry-Reed Relay](http://iotdk.intel.com/docs/master/upm/node/classes/groverelay.html)

DFRobot\* Starter Kit for Intel® Edison, containing:

1. [Moisture Sensor](http://www.dfrobot.com/index.php?route=product/product&product_id=599)
3. [Immersible Pump & Water Tube](http://www.dfrobot.com/index.php?route=product/product&keyword=water pump&product_id=667)
4. [Relay Module](http://www.dfrobot.com/index.php?route=product/product&product_id=64)
5. [I/O Expansion Shield](http://www.dfrobot.com/index.php?route=product/product&product_id=1009)

### Connecting the Grove\* sensors

![](./images/watering-grove.JPG)

You need to have a Grove\* Shield connected to an Arduino\*-compatible breakout board to plug all the Grove\* devices into the Grove\* Shield. Make sure you have the tiny VCC switch on the Grove\* Shield set to **5V**.

You need to power the Intel-based platform with the external power adapter that comes with your starter kit, or substitute it with an external 12V 1.5A power supply. You can also use an external battery, such as a 5V USB battery.

In addition, you need a breadboard and an extra 5V power supply to provide power to the pump. Note: you need a separate battery or power supply for the pump. You cannot use the same power supply for both the Intel-based platform and the pump, so you need either 2 batteries or two power supplies in total.

You need to use the Grove\* Dry-Reed Relay board to connect the water pump.

Sensor | Pin
--- | ---
Grove\* Dry-Reed Relay | D4
One wire from the pump | 5V power source
Other wire from pump | One of the Power connectors on the Grove\* Dry-Reed Relay board
Other power connector  on the Grove\* Dry-Reed Relay board | 5V power source reserved for the pump
Water Flow Sensor | Red wire into the 5V pin, the black wire into the GND pin, and the yellow wire into digital pin 2
Grove\* Moisture Sensor | A0

### Connecting the DFRobot\* sensors

![](./images/watering-dfrobot.JPG)

You need to have a I/O Expansion Shield connected to an Arduino\* compatible breakout board to plug all the DFRobot\* devices into the I/O Expansion Shield.

In addition, you need a breadboard and an extra 5V power supply to provide power to the pump. Note: you need a separate battery or power supply for the pump. You cannot use the same power supply for both the Intel-based platform and the pump.

You need to use the Relay Module to connect the water pump.

Sensor | Pin
--- | ---
Relay Module | A1
One wire from the pump | GND of the power source
Other wire from the pump | NC (Normally Closed) connector on the Relay Module
COM (Common) connector | + of the 5V power source
Moisture Sensor | A3

### Setting the watering schedule

![](./../images/js/watering-system-web.png)

The schedule for the watering system is set using a single-page web interface served from the Intel-based platform while the sample program is running.

The web server runs on port `3000`, so if the Intel-based platform is connected to Wi-Fi on `192.168.1.13`, the address to browse to if you are on the same network is `http://192.168.1.13:3000`.

### Determining your platform's IP Address

See the section on Finding Your Board's IP Address at the bottom of the ![Setup Instructions](./../README.md#finding-your-boards-ip-address) readme. 

IMPORTANT NOTICE: This software is sample software. It is not designed or intended for use in any medical, life-saving or life-sustaining systems, transportation systems, nuclear systems, or for any other mission-critical application in which the failure of the system could lead to critical injury or death. The software may not be fully tested and may contain bugs or errors; it may not be intended or suitable for commercial release. No regulatory approvals for the software have been obtained, and therefore software may not be certified for use in certain countries or environments.
