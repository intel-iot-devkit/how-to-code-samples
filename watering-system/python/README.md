# Watering System in Python*

## Introduction

This Watering System application is part of a series of how-to Internet of Things (IoT) code sample exercises using the Intel® IoT Developer Kit, Intel® Edison development platform, cloud platforms, APIs, and other technologies.

From this exercise, developers will learn how to:<br>
- Connect the Intel® Edison development platform, a computing platform designed for prototyping and producing IoT and wearable computing products.<br>
- Interface with the Intel® Edison platform IO and sensor repository using MRAA and UPM from the Intel® IoT Developer Kit, a complete hardware and software solution to help developers explore the IoT and implement innovative projects.<br>
- Store the Watering System data using Azure Redis Cache\* from Microsoft Azure\*, Redis Store\* from IBM Bluemix\*, or ElastiCache\* using Redis\* from Amazon Web Services (AWS)\*, different cloud services for connecting IoT solutions including data analysis, machine learning, and a variety of productivity tools to simplify the process of connecting your sensors to the cloud and getting your IoT project up and running quickly.
- Connect to a server using IoT Hub from Microsoft Azure\*, IoT from IBM Bluemix\*, IoT from Amazon Web Services (AWS)\*, AT&T\* M2X\*, GE\* Predix\*, or SAP\* Cloud Platform IoT, different cloud-based IoT platforms for machine to machine communication.
- Invoke the services of the Twilio\* API for sending text messages.

## What it is

Using an Intel® Edison board or Intel® IoT Gateway, this project lets you create an automatic watering system that:<br>
- turns a water pump on or off based on a configurable schedule.<br>
- detects if the pumping occurs when expected, by using a water flow sensor.<br>
- can be accessed with your mobile phone via the built-in web interface to set the watering times;<br>
- keeps track of watering events, using cloud-based data storage.<br>
- sends text messages to alert recipients if the system is not working as expected.

## How it works

This watering system allows you to set the watering schedule via a web page served directly from an Intel® Edison board or an Intel® IoT Gateway, by using your mobile phone.

It automatically checks moisture sensor data at periodic intervals, and displays this data on the web page.

If the water pump is supposed to be on but the water flow sensor does not detect that the pumping is talking place as expected, it sends a text message to a specified number through Twilio\* so the watering system can be repaired.

Note that the DFRobot\* Starter Kit does not include a water flow sensor so this sample estimates water flow status based on the moisture sensor when using this kit.

Optionally, data can be stored using your own Microsoft\* Azure\*, IBM\* Bluemix\*, AT&T\* M2X\*, AWS\*, GE\* Predix\*, or SAP\* account.

## Hardware requirements

This sample can be used with either the Grove\* Environment & Agriculture Kit from Seeed Studio, or else the DFRobot\* Edison Starter Kit with some additional DFRobot\* parts.

Grove\* Environment & Agriculture Kit, containing:

1. Intel® Edison board with an Arduino\* breakout board or Intel® IoT Gateway with Arduino 101\* (branded Genuino 101\* outside the U.S.) board
2. [Grove\* Moisture Sensor](http://iotdk.intel.com/docs/master/upm/node/classes/grovemoisture.html)
3. [Water Pump](http://www.seeedstudio.com/depot/6V-Mini-Water-Pump-p-1945.html)
4. [Water Flow Sensor](http://www.seeedstudio.com/depot/G18-Water-Flow-Sensor-p-1346.html)
5. [Grove\* Dry-Reed Relay](http://iotdk.intel.com/docs/master/upm/node/classes/groverelay.html)
6. [Grove\* RGB LCD](http://iotdk.intel.com/docs/master/upm/node/classes/jhd1313m1.html)

DFRobot\* Starter Kit for Intel® Edison, containing:

1. Intel® Edison board with an Arduino\* breakout board or Intel® IoT Gateway with Arduino 101\* (branded Genuino 101\* outside the U.S.) board
2. [Moisture Sensor](http://www.dfrobot.com/index.php?route=product/product&product_id=599)
3. [Immersible Pump & Water Tube](http://www.dfrobot.com/index.php?route=product/product&keyword=water pump&product_id=667)
4. [Relay Module](http://www.dfrobot.com/index.php?route=product/product&product_id=64)
5. [I/O Expansion Shield](http://www.dfrobot.com/index.php?route=product/product&product_id=1009)

## Software requirements

1. Microsoft\* Azure\*, IBM\* Bluemix\*, AT&T\* M2X\*, AWS\*, GE\* Predix\*, or SAP\* account (optional)
2. Twilio\* account (optional)

### How to set up

To begin, clone the **How-To Code Samples** repository with Git\* on your computer as follows:

    $ git clone https://github.com/intel-iot-devkit/how-to-code-samples.git

To download a .zip file, in your web browser go to <a href="https://github.com/intel-iot-devkit/how-to-code-samples">https://github.com/intel-iot-devkit/how-to-code-samples</a> and click the **Download ZIP** button at the lower right. Once the .zip file is downloaded, uncompress it, and then use the files in the directory for this example.

### Installing the program manually on the Intel® Edison board

You can set up the code manually on the Intel® Edison board.

Clone the **How-To Code Samples** repository to your Intel® Edison board after you establish an SSH connection to it, as follows:

    $ git clone https://github.com/intel-iot-devkit/how-to-code-samples.git

Then, navigate to the directory with this example.

To install Git\* on the Intel® Edison board (if you don’t have it yet), establish an SSH connection to the board and run the following command:

    $ opkg install git

### Connecting the Grove\* sensors

![](./../../images/js/watering.jpg)

You need to have a Grove\* Shield connected to an Arduino\*-compatible breakout board to plug all the Grove\* devices into the Grove\* Shield. Make sure you have the tiny VCC switch on the Grove\* Shield set to **5V**.

You need to power the Intel® Edison board with the external power adapter that comes with your starter kit, or substitute it with an external 12V 1.5A power supply. You can also use an external battery, such as a 5V USB battery.

In addition, you need a breadboard and an extra 5V power supply to provide power to the pump. Note: you need a separate battery or power supply for the pump. You cannot use the same power supply for both the Intel® Edison board and the pump, so you need either 2 batteries or two power supplies in total.

You need to use the Grove\* Dry-Reed Relay board to connect the water pump.

1. Plug one end of a Grove\* cable into the Grove\* Dry-Reed Relay, and connect the other end to the D4 port on the Grove\* Shield.

2. Connect one wire from the pump to the 5V power source reserved for the pump.

3. Connect the other wire from the pump to one of the power connectors on the Grove\* Dry-Reed Relay board.

4. Connect the other power connector on the Grove\* Dry-Reed Relay board to the ground of the 5V power source reserved for the pump.

5. Connect the Water Flow Sensor by plugging the red wire into the 5V pin, the black wire into the GND pin, and the yellow wire into digital pin 2 on the Grove\* Shield.

6. Plug one end of a Grove\* cable into the Grove\* Moisture Sensor, and connect the other end to the A3 port on the Grove\* Shield.

7. Plug one end of a Grove\* cable into the Grove\* RGB LCD, and connect the other end to any of the I2C ports on the Grove\* Shield.

### Connecting the DFRobot\* sensors

![](./../../images/js/watering-dfrobot.jpg)

You need to have a I/O Expansion Shield connected to an Arduino\* compatible breakout board to plug all the DFRobot\* devices into the I/O Expansion Shield.

In addition, you need a breadboard and an extra 5V power supply to provide power to the pump. Note: you need a separate battery or power supply for the pump. You cannot use the same power supply for both the Intel® Edison board and the pump.

You need to use the Relay Module to connect the water pump.

1. Plug one end of a DFRobot\* cable into the Relay Module, and connect the other end to the A1 port on the I/O Expansion Shield.

2. Connect one wire from the pump to the GND of the power source reserved for the pump.

3. Connect the other wire from the pump to the NC (Normally Closed) connector on the Relay Module.

4. Connect the COM (Common) connector on the Relay Module to the + of the 5V power source reserved for the pump.

5. Plug one end of a DFRobot\* cable into the Moisture Sensor, and connect the other end to the A3 port on the I/O Expansion Shield.

### Intel® Edison board setup

If you're running this code on your Intel® Edison board, you need to install some dependencies by establishing an SSH session to the Edison and run the commands in the sections below.

#### Update the opkg repo

To add the Intel opkg repository:

    $ echo "src mraa-upm http://iotdk.intel.com/repos/3.5/intelgalactic/opkg/i586" > /etc/opkg/mraa-upm.conf
    $ opkg update

You'll only need to perform this step once.

#### Git\*

To install Git\* on the Intel® Edison board (if you don’t have it yet):

    $ opkg update
    $ opkg install git

#### MRAA and UPM Dependencies

To install the latest versions of the MRAA and UPM libraries:

    $ opkg update
    $ opkg install mraa
    $ opkg install upm

#### Python\* Package Manager (pip)

To install the Python\* package manager needed to install and run the example:

    $ pip install --upgrade pip setuptools

#### Install the example

Once all dependencies are installed you can install the example itself with the following command:

    $ pip install --src ~/python/examples/ -e "git+https://github.com/intel-iot-devkit/how-to-code-samples.git#egg=iot_watering_system&subdirectory=watering-system/python"

The `pip` command will install required Python dependencies, save the source code for the example in `~/python/examples/iot_watering_system/` and link the package to the global Python `site-packages` folder.

### Intel® IoT Gateway setup

You can run this example using an Intel® IoT Gateway connected to an Arduino 101\*(branded Genuino 101\* outside the U.S.) board.

Make sure your Intel® IoT Gateway is setup, by following the directions on the web site here:

https://software.intel.com/en-us/getting-started-with-intel-iot-gateways-and-iotdk

The Arduino 101\*(branded Genuino 101\* outside the U.S.) board needs to have the Firmata\* firmware installed. If you have IMRAA installed on your gateway, this will be done automatically. Otherwise, install the StandardFirmata or ConfigurableFirmata sketch manully on to your Arduino 101\*(branded Genuino 101\* outside the U.S.) board.

You will also need to configure the `config.json` in the example to use the Arduino 101\*(branded Genuino 101\* outside the U.S.) board. See the section "Configuring the example" below.

### Twilio\* API key

To optionally send text messages, you need to register for an account and get an API key from the Twilio\* web site:

<a href="https://www.twilio.com">https://www.twilio.com</a>

You cannot send text messages without obtaining a Twilio\* API key first. You can still run the example, but without the text messages.

Pass your Twilio\* API key and authentication token to the sample program by modifying the `TWILIO_ACCT_SID` and `TWILIO_AUTH_TOKEN` keys in the `config.json` file as follows:

```JSON
{
  "TWILIO_ACCT_SID": "YOURAPIKEY",
  "TWILIO_AUTH_TOKEN": "YOURTOKEN"
}
```

### IoT cloud setup

You can optionally store the data generated by this sample program using cloud-based IoT platforms from Microsoft\* Azure\*, IBM\* Bluemix*, AT&T\* M2X\*, AWS\*, GE\* Predix\*, or SAP\*.

For information on how to connect to your own cloud server, go to:

[https://github.com/intel-iot-devkit/iot-samples-cloud-setup](https://github.com/intel-iot-devkit/iot-samples-cloud-setup)

### Data store server setup

Optionally, you can store the data generated by this sample program in a back-end database deployed using Microsoft Azure\*, IBM Bluemix\*, or AWS\*, along with Node.js\*, and a Redis\* data store.

For information on how to set up your own cloud data server, go to:

[https://github.com/intel-iot-devkit/intel-iot-examples-datastore](https://github.com/intel-iot-devkit/intel-iot-examples-datastore)

## Configuring the example

When the example is installed through `pip` the `config.json` file that holds the configuration for the example lives in `~/python/examples/iot_watering_system/watering-system/python/iot_watering_system/config.json`.

To configure the example for the Grove\* kit, just leave the `kit` key in the `config.json` set to `grove`. To configure the example for the DFRobot\* kit, change the `kit` key in the `config.json` to `dfrobot` as follows:

```JSON
{
  "kit": "dfrobot"
}
```

To configure the example for the Arduino 101\*(branded Genuino 101\* outside the U.S.) board, add a `PLATFORM` key with the value `firmata` to the `config.json`, as follows:

```JSON
{
  "kit": "grove",
  "PLATFORM": "firmata"
}
```

To configure the example for sending optional text messages, obtain an API key from the Twilio\* website as explained above, and then change the `TWILIO_ACCT_SID` and `TWILIO_AUTH_TOKEN` keys in the `config.json` file as follows:

```JSON
{
  "kit": "grove",
  "TWILIO_ACCT_SID": "YOURAPIKEY",
  "TWILIO_AUTH_TOKEN": "YOURTOKEN"
}
```

To configure the example for the optional Microsoft Azure\*, IBM Bluemix\*, or AWS\* data store, add the `SERVER` and `AUTH_TOKEN` keys in the `config.json` file below the "CODE" key as follows:

```JSON
{
  "kit": "grove",
  "SERVER": "http://intel-examples.azurewebsites.net/logger/access-control",
  "AUTH_TOKEN": "s3cr3t"
}
```

For information on how to configure the example for an optional Microsoft\* Azure\*, IBM\* Bluemix\*, AT&T\* M2X\*, AWS\*, GE\* Predix\*, or SAP\* IoT cloud server, go to:

[https://github.com/intel-iot-devkit/iot-samples-cloud-setup](https://github.com/intel-iot-devkit/iot-samples-cloud-setup)

## Running the program manually

Once the example is installed through `pip` you can run the program by running the following command in an SSH session to the board:

    $ python -m iot_watering_system

### Determining the Intel® Edison board's IP address

You can determine what IP address the Intel® Edison board is connected to by running the following command:

    ip addr show | grep wlan

You will see the output similar to the following:

    3: wlan0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast qlen 1000
        inet 192.168.1.13/24 brd 192.168.1.255 scope global wlan0

The IP address is shown next to `inet`. In the example above, the IP address is `192.168.1.13`.

IMPORTANT NOTICE: This software is sample software. It is not designed or intended for use in any medical, life-saving or life-sustaining systems, transportation systems, nuclear systems, or for any other mission-critical application in which the failure of the system could lead to critical injury or death. The software may not be fully tested and may contain bugs or errors; it may not be intended or suitable for commercial release. No regulatory approvals for the software have been obtained, and therefore software may not be certified for use in certain countries or environments.
