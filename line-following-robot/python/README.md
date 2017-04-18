# Line Following Robot in Python*

## Introduction

This Line Following Robot application is part of a series of how-to Internet of Things (IoT) code sample exercises using the Intel® IoT Developer Kit, Intel® Edison board, Intel® IoT Gateway, cloud platforms, APIs, and other technologies.

From this exercise, developers will learn how to:<br>
- Connect the Intel® Edison board or Intel® IoT Gateway, computing platforms designed for prototyping and producing IoT and wearable computing products.<br>
- Interface with the Intel® Edison board or Arduino 101\* (branded Genuino 101\* outside the U.S.) IO and sensor repository using MRAA and UPM from the Intel® IoT Developer Kit, a complete hardware and software solution to help developers explore the IoT and implement innovative projects.<br>
- Set up a web application server to control a robot arm using a web page served directly from the Intel® Edison board or Intel® IoT Gateway.
- Store the Line Following Robot data using Azure Redis Cache\* from Microsoft Azure\*, Redis Store\* from IBM Bluemix\*, or ElastiCache\* using Redis\* from Amazon Web Services\* (AWS), different cloud services for connecting IoT solutions including data analysis, machine learning, and a variety of productivity tools to simplify the process of connecting your sensors to the cloud and getting your IoT project up and running quickly.
- Set up a MQTT-based server using IoT Hub from Microsoft Azure\*, IoT from IBM Bluemix\*, or IoT from Amazon Web Services\* (AWS)\*, different cloud machine to machine messaging services based on the industry standard MQTT protocol.

## What it is

Using an Intel® Edison board or Intel® IoT Gateway, this project lets you create a line following robot that:<br>
- continuously checks the line finder sensor.<br>
- moves forward if on the line, using the stepper motors.<br>
- tries to pivot to find the line if not on it, using the stepper motors.<br>
- logs events from the line finder sensor, using cloud-based data storage.

## How it works

The line finder robot uses the two attached motors to attempt to follow a line, keeping track of it with the line finder sensor. If on the line, it moves forward. Otherwise, it pivots in place, trying to locate it using the line finder sensor.

Optionally, all data can be stored using the Intel® IoT Examples Data store or an MQTT server running in your own Microsoft Azure\*, IBM Bluemix\*, or AWS account.

## Hardware requirements

Grove* Robotics Kit containing:

1. Intel® Edison with an Arduino-compatible breakout board or Intel® IoT Gateway with Intel® Arduino 101
2. [Grove Line Finder](http://iotdk.Intel®.com/docs/master/upm/node/classes/grovelinefinder.html)
3. [Stepper Motor Controller & Stepper Motor](http://iotdk.Intel®.com/docs/master/upm/node/classes/uln200xa.html) (x2)

## Software requirements

1. Microsoft Azure\*, IBM Bluemix\*, or AWS\* account (optional)

### Connecting the Grove\* sensors

![](./../../images/js/line-follower.jpg)

You need to have a Grove* Shield connected to an Arduino\*-compatible breakout board to plug all the Grove devices into the Grove Shield. Make sure you have the tiny VCC switch on the Grove Shield set to **5V**.

You need to power the Intel® Edison board with the external power adapter that comes with your starter kit, or substitute it with an external 12V 1.5A power supply. You can also use an external battery, such as a 5V USB battery.

In addition, you need a breadboard and an extra 5V power supply to provide power to both motors. Note: you need a separate battery or power supply for the motors. You cannot use the same power supply for both the Intel® Edison board and the motors, so you need either 2 batteries or 2 power supplies in total.

1. Plug each of the stepper motor controllers into four pins on the Arduino* breakout board for it to be able to be controlled. Connect stepper motor controller #1 to pins 4, 5, 6, and 7. Connect stepper motor controller #2 to pins 9, 10, 11, and 12. Connect both controllers to ground (GND), to the 5V power coming from the Arduino* breakout board (VCC), and to the separate 5V power for the motors (VM).

2. Plug one end of a Grove* cable into the Grove* Line Finder, and connect the other end to the D2 port on the Grove* Shield.

### Manual Intel® Edison setup

If you're running this code on your Intel® Edison board manually, you need to install some dependencies by establishing an SSH session to the board running the following commands.

To install Git\* on the Intel® Edison board (if you don’t have it yet), establish an SSH connection to the board and run the following command:

    $ opkg install git

To obtain the Python\* packages needed for this example to execute on the Intel® Edison board:

Run the following commands to update to the latest version of Python's package installer:

    $ pip install --upgrade pip setuptools

Run the following to update to the latest versions of the Intel MRAA and UPM libaries:

    $ echo "src mraa-upm http://iotdk.intel.com/repos/3.5/intelgalactic/opkg/i586" > /etc/opkg/mraa-upm.conf
    $ opkg update
    $ opkg install mraa upm

Once the dependencies are installed you can install the example itself with the following command:

    $ pip install --src ~/python/examples/ -e "git+https://github.com/intel-iot-devkit/how-to-code-samples.git#egg=iot_line_following_robot&subdirectory=line-following-robot/python"

The `pip` command will install required Python dependencies, save the source code for the example in `~/python/examples/iot_line_following_robot/` and link the package to the global Python `site-packages` folder.

### Intel® IoT Gateway setup

You can run this example using an Intel® IoT Gateway connected to an Arduino 101\*(branded Genuino 101\* outside the U.S.).

Make sure your Intel® IoT Gateway is setup using Intel® IoT Gateway Software Suite, by following the directions on the web site here:

https://software.intel.com/en-us/getting-started-with-intel-iot-gateways-and-iotdk

To install and run the example, you will need to install Python setuptools. This is easy to do, by running:

    $ wget https://bootstrap.pypa.io/ez_setup.py -O - | python

Once these dependencies are installed you can install the example itself with the following command:

    $ pip install --src ~/python/examples/ -e "git+https://github.com/intel-iot-devkit/how-to-code-samples.git#egg=iot_line_following_robot&subdirectory=line-following-robot/python"    

The `pip` command will install required Python dependencies, save the source code for the example in `~/python/examples/iot_line_following_robot/` and link the package to the global Python `site-packages` folder.

The Arduino 101 needs to have the Firmata\* firmware installed. If you have IMRAA installed on your gateway, this will be done automatically. Otherwise, install the StandardFirmata or ConfigurableFirmata sketch manually on to your Arduino 101.

You will also need to configure the `config.json` in the example to use the Arduino 101. See the section "Configuring the example" below.

### Data store server setup

Optionally, you can store the data generated by this sample program in a back-end database deployed using Microsoft Azure\*, IBM Bluemix\*, or AWS\*, along with Node.js\*, and a Redis\* data store.

For information on how to set up your own cloud data server, go to:

[https://github.com/intel-iot-devkit/intel-iot-examples-datastore](https://github.com/intel-iot-devkit/intel-iot-examples-datastore)

### MQTT server setup

You can also optionally store the data generated by this sample program using MQTT, a machine-to-machine messaging server. You can use MQTT to connect to Microsoft Azure\*, IBM Bluemix\*, or AWS\*.

For information on how to connect to your own cloud MQTT messaging server, go to:

[https://github.com/intel-iot-devkit/intel-iot-examples-mqtt](https://github.com/intel-iot-devkit/intel-iot-examples-mqtt)

## Configuring the example

When the example is installed through `pip` the `config.json` file that holds the configuration for the example lives in `~/python/examples/iot_line_following_robot/line-following-robot/python/iot_line_following_robot/config.json`.

To configure the example for the Arduino 101\*, add a `PLATFORM` key with the value `firmata` to the `config.json`, as follows:

```JSON
{
  "kit": "grove",
  "PLATFORM": "firmata"
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

Once the example is installed through `pip` you can run the program by running the following command in an SSH session to the board:

    $ python -m iot_line_following_robot

### Determining the Intel® Edison board's IP address

You can determine what IP address the Intel® Edison board is connected to by running the following command:

    ip addr show | grep wlan

You will see the output similar to the following:

    3: wlan0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast qlen 1000
        inet 192.168.1.13/24 brd 192.168.1.255 scope global wlan0

The IP address is shown next to `inet`. In the example above, the IP address is `192.168.1.13`.

IMPORTANT NOTICE: This software is sample software. It is not designed or intended for use in any medical, life-saving or life-sustaining systems, transportation systems, nuclear systems, or for any other mission-critical application in which the failure of the system could lead to critical injury or death. The software may not be fully tested and may contain bugs or errors; it may not be intended or suitable for commercial release. No regulatory approvals for the software have been obtained, and therefore software may not be certified for use in certain countries or environments.
