# Range Finder Scanner

## Introduction

This range finder scanner application is part of a series of how-to Internet of Things (IoT) code sample exercises using the Intel® IoT Developer Kit, and a compatible Intel-based platform, cloud platforms, APIs, and other technologies.

From this exercise, developers will learn how to:<br>
- Interface with and sensors using MRAA and UPM from the Intel® IoT Developer Kit, a complete hardware and software solution to help developers explore the IoT and implement innovative projects.<br>
- Set up a web application server to view range finder data using a web page served directly from the Intel-based platform.

Available in:  
 ![C++](./cpp)  
 ![Java\*](./java)  
 ![JavaScript\*](./javascript)  
 ![Python\*](./python) 

## What it is

Using a compatible Intel-based platform, this project lets you create a range finding scanner that:<br>
- continuously checks the Grove\* IR Distance Interrupter.<br>
- moves the stepper motor in a 360-degree circle.<br>
- can be accessed via the built-in web interface to view range finder data.

## How it works

As the stepper motor turns, it pauses to get readings from the Grove\* IR Distance Interrupter.

These readings can be seen by viewing the web page served directly from the Intel-based platform.

## First time setup  
For all the samples in this repository, see the ![General Setup Instructions](./../README.md#setup) for required boards and libraries.

## Hardware requirements

In addition to using a compatible platform listed in ![Hardware Compatibility](./../README.md#hardware-compatibility), here is additional hardware you will need to run this example.

Grove\* Robotics Kit containing:

1. [Grove\* Base Shield V2](https://www.seeedstudio.com/Base-Shield-V2-p-1378.html)
2. [Grove\* IR Distance Interrupter](http://iotdk.intel.com/docs/master/upm/node/classes/rfr359f.html)
3. [Stepper Motor Controller & Stepper Motor](http://iotdk.intel.com/docs/master/upm/node/classes/uln200xa.html)

### Connecting the Grove\* sensors

![](./images/range-finder-grove.JPG)

You need to have a Grove\* Shield connected to an Arduino\* compatible breakout board to plug all the Grove devices into the Grove Shield. Make sure you have the tiny VCC switch on the Grove Shield set to **5V**.

You need to power the Intel-based platform with the external power adaptor that comes with your starter kit, or substitute it with an external 12V 1.5A power supply. You can also use an external battery, such as a 5V USB battery.

In addition, you need a breadboard and an extra 5V power supply to provide power to the motor. Note: you need a separate battery or power supply for the motor. You cannot use the same power supply for both the Intel-based platform and the motor, so you need either 2 batteries or 2 power supplies in total.

Sensor | Pin
--- | ---
Stepper motor controller | Digital pins 9, 10, 11, and 12
Controller to ground (GND) | 5V (VCC) and 5V (VM)

Look at the bottom of the shield to determine the pin numbers.

### Viewing the range data

![](./../../images/js/range-finder-web.png)

The range finder data is viewed using a single-page web interface served from the Intel-based platform while the sample program is running.

The web server runs on port `3000`, so if the Intel-based platform is connected to Wi-Fi on `192.168.1.13`, the address to browse to if you are on the same network is `http://192.168.1.13:3000`.

### Determining your platform's IP Address

See the section on Finding Your Board's IP Address at the bottom of the ![Setup Instructions](./../README.md#finding-your-boards-ip-address) readme. 

IMPORTANT NOTICE: This software is sample software. It is not designed or intended for use in any medical, life-saving or life-sustaining systems, transportation systems, nuclear systems, or for any other mission-critical application in which the failure of the system could lead to critical injury or death. The software may not be fully tested and may contain bugs or errors; it may not be intended or suitable for commercial release. No regulatory approvals for the software have been obtained, and therefore software may not be certified for use in certain countries or environments.
