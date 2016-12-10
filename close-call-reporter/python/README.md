# Close Call Reporter in Python*

## Introduction

This Close Call Reporter application is part of a series of how-to Intel® Internet of Things (IoT) code sample exercises using the Intel® IoT Developer Kit, Intel® Edison development platform, cloud platforms, APIs, and other technologies.

From this exercise, developers will learn how to:<br>
- Connect the Intel® Edison development platform, a computing platform designed for prototyping and producing IoT and wearable computing products.<br>
- Interface with the Intel® Edison platform IO and sensor repository using MRAA and UPM from the Intel® IoT Developer Kit, a complete hardware and software solution to help developers explore the IoT and implement innovative projects.<br>
- Store the Close Call Reporter data using Azure Redis Cache\* from Microsoft Azure\*, Redis Store\* from IBM Bluemix\*, or ElastiCache\* using Redis\* from Amazon Web Services (AWS)\*, different cloud services for connecting IoT solutions including data analysis, machine learning, and a variety of productivity tools to simplify the process of connecting your sensors to the cloud and getting your IoT project up and running quickly.
- Set up a MQTT-based server using IoT Hub from Microsoft Azure\*, IoT from IBM Bluemix\*, or IoT from Amazon Web Services (AWS)\*, different cloud machine to machine messaging services based on the industry standard MQTT protocol.

## What it is

Using an Intel® Edison board, this project lets you create a close call fleet driving reporter that:<br>
- monitors the Grove* IR Distance Interrupter.<br>
- monitors the Grove* GPS.<br>
- keeps track of close calls and logs them using cloud-based data storage.

## How it works

This close call reporter system monitors the direction the IR Distance sensor is pointed to.

It also keeps track of the GPS position of the Intel® Edison board, updating the position frequently to ensure accurate data.

If a close call is detected (that is, the IR Distance sensor is tripped), the Intel® Edison board, if configured, notifies the Intel® IoT Examples Datastore or an MQTT server running in your own Microsoft Azure\*, IBM Bluemix\*, or AWS\* account.

## Hardware requirements

This sample can be used with either the Grove\* Transportation and Safety Kit from Seeed Studio\*, or else the Starter Kit for Intel® Edison/Galileo from DFRobot\*.

Grove\* Transportation and Safety Kit, containing:

1. Intel® Edison with an Arduino-compatible breakout board
2. [Grove\* IR Distance Interrupter](http://iotdk.intel.com/docs/master/upm/node/classes/rfr359f.html)
3. [Grove\* GPS](http://iotdk.intel.com/docs/master/upm/node/classes/ublox6.html)

DFRobot* Starter Kit for Intel® Edison, containing:

1. Intel® Edison with an Arduino* breakout board
2. [IR Distance Sensor](http://www.dfrobot.com/index.php?route=product/product&product_id=572)
3. [GPS](http://iotdk.intel.com/docs/master/upm/node/classes/ublox6.html)
4. [I/O Expansion Shield](http://www.dfrobot.com/index.php?route=product/product&product_id=1009)

## Software requirements

1. Microsoft Azure\*, IBM Bluemix\*, or AWS\* account (optional)

### Connecting the Grove\* sensors

![](./../../images/js/close-call.jpg)

You need to have a Grove\* Shield connected to an Arduino-compatible breakout board to plug all the Grove\* devices into the Grove\* Shield. Make sure you have the tiny VCC switch on the Grove\* Shield set to **5V**.

1. Plug one end of a Grove\* cable into the Grove\* IR Distance Interrupter, and connect the other end to the D2 port on the Grove\* Shield.

2. Plug one end of a Grove\* cable into the Grove\* GPS, and connect the other end to the UART port on the Grove\* Shield.

### Connecting the DFRobot\* sensors

![](./../../images/js/close-call-dfrobot.jpg)

You need to have a DFRobot\* I/O Expansion Shield connected to an Arduino\*-compatible breakout board to plug all the DFRobot\* devices into the DFRobot\* I/O Expansion Shield.

1. Plug one end of a DFRobot\* cable into the IR Distance Sensor, and connect the other end to the D4 port on the I/O Expansion Shield.

2. Plug the attached GPS cable TX (white) to the I/O Expansion Shield's RX pin. Plug the attached GPS cable RX (black) to the I/O Expansion Shield's TX pin. Plug the attached GPS cable power (red) to any of the I/O Expansion Shield's 5V pins. Plug the attached GPS cable ground (thicker black) to any of the I/O Expansion Shield's GND.

### Intel® Edison board setup

If you're running this code on your Intel® Edison board, you need to install some dependencies by establishing an SSH session to the Edison and run the commands in the sections below.

#### Update the opkg repo

To add the Intel opkg repository:

    $ echo "src mraa-upm http://iotdk.intel.com/repos/3.5/intelgalactic/opkg/i586" > /etc/opkg/mraa-upm.conf
    $ opkg update

You'll only need to perform this step once.

#### Git

To install Git\* on the Intel® Edison board (if you don’t have it yet):

    $ opkg update
    $ opkg install git

#### MRAA and UPM Dependencies

To install the latest versions of the MRAA\* and UPM\* libraries:

    $ opkg update
    $ opkg install mraa
    $ opkg install upm

#### Python Package Manager (pip)

To install the Python\* package manager needed to install and run the example:

    $ pip install --upgrade pip setuptools

#### Install the example

Once all dependencies are installed you can install the example itself with the following command:

    $ pip install --src ~/python/examples/ -e "git+https://github.com/intel-iot-devkit/how-to-code-samples.git#egg=iot_close_call_reporter&subdirectory=close-call-reporter/python"

The `pip` command will install required Python dependencies, save the source code for the example in `~/python/examples/iot_close_call_reporter/` and link the package to the global Python `site-packages` folder.

### Data store server setup

Optionally, you can store the data generated by this sample program in a back-end database deployed using Microsoft Azure\*, IBM Bluemix\*, or AWS\*, along with Node.js\*, and a Redis\* data store.

For information on how to set up your own cloud data server, go to:

[https://github.com/intel-iot-devkit/intel-iot-examples-datastore](https://github.com/intel-iot-devkit/intel-iot-examples-datastore)

### MQTT server setup

You can also optionally store the data generated by this sample program using MQTT, a machine-to-machine messaging server. You can use MQTT to connect to Microsoft Azure\*, IBM Bluemix\*, or AWS\*.

For information on how to connect to your own cloud MQTT messaging server, go to:

[https://github.com/intel-iot-devkit/intel-iot-examples-mqtt](https://github.com/intel-iot-devkit/intel-iot-examples-mqtt)

## Configuring the example

When the example is installed through `pip` the `config.json` file that holds the configuration for the example lives in `~/python/examples/iot_close_call_reporter/close-call-reporter/python/iot_close_call_reporter/config.json`.

To configure the GPS hardware for the example set the `GPS_BAUD` config key in `config.json` to the UART baud rate used by your specific GPS device. The `GPS_BAUD` config value is set to `9600` by default.

```JSON
{
    "kit": "grove",
    "GPS_BAUD": 9600
}
```

The table below contains the GPS baud rates for the common GPS hardware used with this example:

| GPS Hardware                   | UART Baud Rate |
| ------------------------------ | -------------- |
| Grove GPS                      | 9600           |
| DFRobot GPS Receiver (Model A) | 38400          |
| DFRobot GPS Receiver (Model B) | 115200         |

To configure the example for the Grove\* kit, just leave the `kit` key in the `config.json` set to `grove`. To configure the example for the DFRobot\* kit, change the `kit` key in the `config.json` to `dfrobot` as follows:

```JSON
{
  "kit": "dfrobot"
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

For information on how to configure the example for the optional Microsoft Azure\*, IBM Bluemix\*, or AWS\* MQTT messaging server, go to:

[https://github.com/intel-iot-devkit/intel-iot-examples-mqtt/](https://github.com/intel-iot-devkit/intel-iot-examples-mqtt/)

## Running the program

Once the program is installed and configured as detailed above, you can execute the program by running the following command in an SSH session to the board:

    $ python -m iot_close_call_reporter

### Determining the Intel® Edison board's IP address

You can determine what IP address the Intel® Edison board is connected to by running the following command:

    ip addr show | grep wlan

You will see the output similar to the following:

    3: wlan0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast qlen 1000
        inet 192.168.1.13/24 brd 192.168.1.255 scope global wlan0

The IP address is shown next to `inet`. In the example above, the IP address is `192.168.1.13`.

IMPORTANT NOTICE: This software is sample software. It is not designed or intended for use in any medical, life-saving or life-sustaining systems, transportation systems, nuclear systems, or for any other mission-critical application in which the failure of the system could lead to critical injury or death. The software may not be fully tested and may contain bugs or errors; it may not be intended or suitable for commercial release. No regulatory approvals for the software have been obtained, and therefore software may not be certified for use in certain countries or environments.
