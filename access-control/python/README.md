# Access control in Python*

## Introduction

This access control system application is part of a series of how-to Intel® Internet of Things (IoT) code sample exercises using the Intel® IoT Developer Kit, Intel® Edison board, Intel® IoT Gateway, cloud platforms, APIs, and other technologies.

From this exercise, developers will learn how to:<br>
- Connect the Intel® Edison board or Intel® IoT Gateway, computing platforms designed for prototyping and producing IoT and wearable computing products.<br>
- Interface with the Intel® Edison board or Arduino 101\* (branded Genuino 101\* outside the U.S.) IO and sensor repository using MRAA and UPM from the Intel® IoT Developer Kit, a complete hardware and software solution to help developers explore the IoT and implement innovative projects.<br>
- Set up a web application server to let users enter the access code to disable the alarm system and store this alarm data using Azure Redis Cache\* from Microsoft\* Azure\*, Redis Store\* from IBM\* Bluemix\*, or ElastiCache\* using Redis\* from Amazon\* Web Services\* (AWS\*), different cloud services for connecting IoT solutions including data analysis, machine learning, and a variety of productivity tools to simplify the process of connecting your sensors to the cloud and getting your IoT project up and running quickly.
- Set up a MQTT-based server using IoT Hub from Microsoft\* Azure\*, IoT from IBM\* Bluemix\*, or IoT from Amazon Web Services\* (AWS), different cloud machine to machine messaging services based on the industry standard MQTT protocol.

## What it is

Using an Intel® Edison board or Intel® IoT Gateway, this project lets you create a smart access control system that:<br>
- monitors a motion sensor to detect when a person is in an area that requires authorization.<br>
- can be accessed with your mobile phone via the built-in web interface to disable the alarm.<br>
- keeps track of access, using cloud-based data storage.

## How it works

This access control system provides the following user flow:

1. Passive infrared (PIR) motion sensor looks for motion.
2. User sets off the motion detector and has 30 seconds to enter the correct code in the browser.
3. If the user fails to enter the code in the given time, the alarm goes off.
4. If the user enters the correct code, the system waits for 30 seconds before allowing the user to pass.

Additionally, various events (`looking-for-motion`, `motion-detected`, `invalid-code`, etc.) are logged.

Optionally, all data can be stored using the Intel® IoT Examples data store or an MQTT server running in your own Microsoft\* Azure\*, IBM\* Bluemix\*, or AWS account.

## Hardware requirements

This sample can be used with either the Grove\* Transportation and Safety Kit from Seeed Studio, or else the DFRobot\* Edison Starter Kit.

Grove\* Transportation and Safety Kit, containing:

1. Intel® Edison board with an Arduino* breakout board
2. [Grove\* PIR Motion Sensor](http://iotdk.intel.com/docs/master/upm/node/classes/biss0001.html)
3. [Grove\* RGB LCD](http://iotdk.intel.com/docs/master/upm/node/classes/jhd1313m1.html)

DFRobot\* Starter Kit for Intel® Edison, containing:

1. Intel® Edison with an Arduino\* breakout board
2. [PIR (Motion) Sensor](http://iotdk.intel.com/docs/master/upm/node/classes/biss0001.html).
3. [LCD Keypad Shield](http://iotdk.intel.com/docs/master/upm/node/classes/sainsmartks.html)

## Software requirements

1. Microsoft\* Azure\*, IBM\* Bluemix\*, or AWS account (optional)

### Connecting the Grove\* sensors

![](./../../images/js/access-control.jpg)

You need to have a Grove\* Shield connected to an Arduino\*-compatible breakout board to plug all the Grove devices into the Grove\* Shield. Make sure you have the tiny VCC switch on the Grove\* Shield set to **5V**.

1. Plug one end of a Grove\* cable into the Grove\* PIR Motion Sensor, and connect the other end to the D4 port on the Grove\* Shield.

2. Plug one end of a Grove\* cable into the Grove RGB LCD, and connect the other end to any of the I2C ports on the Grove\* Shield.

### Connecting the DFRobot\* sensors

![](./../../images/js/access-control-dfrobot.jpg)

You need to have a LCD Display Shield connected to an Arduino\*-compatible breakout board to plug all the DFRobot\* devices into the LCD Display Shield.

1. Plug one end of a DFRobot\* cable into the PIR (Motion) Sensor, and connect the other end to the A2 port on the LCD Display Shield.

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

    $ pip install --src ~/python/examples/ -e "git+https://github.com/intel-iot-devkit/how-to-code-samples.git#egg=iot_access_control&subdirectory=access-control/python"

The `pip` command will install required Python dependencies, save the source code for the example in `~/python/examples/iot-access-control/` and link the package to the global Python `site-packages` folder.

### Intel® IoT Gateway setup

You can run this example using an Intel® IoT Gateway connected to an Arduino\*/Genuino\* 101.

Make sure your Intel® IoT Gateway is setup, by following the directions on the web site here:

https://software.intel.com/en-us/node/633284

To install and run the example, you will need to install Python setuptools. This is easy to do, by running:

    $ wget https://bootstrap.pypa.io/ez_setup.py -O - | python

Once these dependencies are installed you can install the example itself with the following command:

    $ pip install --src ~/python/examples/ -e "git+https://github.com/intel-iot-devkit/how-to-code-samples.git#egg=iot_access_control&subdirectory=access-control/python"    

The `pip` command will install required Python dependencies, save the source code for the example in `~/python/examples/iot-access-control/` and link the package to the global Python `site-packages` folder.

The Arduino\*/Genuino\* 101 needs to have the Firmata\* firmware installed. If you have IMRAA installed on your gateway, this will be done automatically. Otherwise, install the StandardFirmata or ConfigurableFirmata sketch manually on to your Arduino\*/Genuino\* 101.

You will also need to configure the `config.json` in the example to use the Arduino\*/Genuino\* 101. See the section "Configuring the example" below.

### Data store server setup

Optionally, you can store the data generated by this sample program in a back-end database deployed using Microsoft\* Azure\*, IBM\* Bluemix\*, or AWS, along with Node.js\*, and a Redis\* data store.

For information on how to set up your own cloud data server, go to:

[https://github.com/intel-iot-devkit/intel-iot-examples-datastore](https://github.com/intel-iot-devkit/intel-iot-examples-datastore)

### MQTT server setup

You can also optionally store the data generated by this sample program using MQTT, a machine-to-machine messaging server. You can use MQTT to connect to Microsoft\* Azure\*, IBM\* Bluemix\*, or AWS.

For information on how to connect to your own cloud MQTT messaging server, go to:

[https://github.com/intel-iot-devkit/intel-iot-examples-mqtt](https://github.com/intel-iot-devkit/intel-iot-examples-mqtt)

## Configuring the example

When the example is installed through `pip` the `config.json` file that holds the configuration for the example lives in `~/python/examples/iot_access_control/access-control/python/iot_access_control/config.json`.

To configure the example for the Grove* kit, just leave the `kit` key in the `config.json` set to `grove`. To configure the example for the DFRobot* kit, change the `kit` key in the `config.json` to `dfrobot` as follows:

```JSON
{
  "kit": "dfrobot",
  "CODE": "4321"
}
```

To configure the example for the Arduino\*/Genuino\* 101, add a `PLATFORM` key with the value `firmata` to the `config.json`, as follows:

```JSON
{
  "kit": "dfrobot",
  "CODE": "4321",
  "PLATFORM": "firmata"
}
```

To configure the example for the optional Microsoft\* Azure\*, IBM\* Bluemix\*, or AWS data store, add the `SERVER` and `AUTH_TOKEN` keys in the `config.json` file below the "CODE" key as follows:

```JSON
{
  "kit": "grove",
  "CODE": "4321",
  "SERVER": "http://intel-examples.azurewebsites.net/logger/access-control",
  "AUTH_TOKEN": "s3cr3t"
}
```

For information on how to configure the example for the optional Microsoft\* Azure\*, IBM\* Bluemix\*, or AWS MQTT messaging server, go to:

[https://github.com/intel-iot-devkit/intel-iot-examples-mqtt/](https://github.com/intel-iot-devkit/intel-iot-examples-mqtt/)

## Running the program

Once the program is installed and configured as detailed above, you can execute the program by running the following command in an SSH session to the board:

    $ python -m iot_access_control

### Disabling the alarm

![](./../../images/js/access-control-web.png)

The alarm is disabled using a single-page web interface served directly from the Intel® Edison board while the sample program is running.

The web server runs on port `3000`, so if the Intel® Edison board is connected to Wi-Fi* on `192.168.1.13`, the address to browse to if you are on the same network is `http://192.168.1.13:3000`.

### Determining the Intel® Edison board's IP address

You can determine what IP address the Intel® Edison board is connected to by running the following command:

    ip addr show | grep wlan

You will see output similar to the following:

    3: wlan0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast qlen 1000
        inet 192.168.1.13/24 brd 192.168.1.255 scope global wlan0

The IP address is shown next to `inet`. In the example above, the IP address is `192.168.1.13`.

IMPORTANT NOTICE: This software is sample software. It is not designed or intended for use in any medical, life-saving or life-sustaining systems, transportation systems, nuclear systems, or for any other mission-critical application in which the failure of the system could lead to critical injury or death. The software may not be fully tested and may contain bugs or errors; it may not be intended or suitable for commercial release. No regulatory approvals for the software have been obtained, and therefore software may not be certified for use in certain countries or environments.
