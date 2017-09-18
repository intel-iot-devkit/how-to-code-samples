# Watering system in Java*

## Introduction

This automatic watering system application is part of a series of how-tos for Internet of Things (IoT) code sample exercises using the Intel® IoT Developer Kit, Intel® Edison board, Intel® IoT Gateway, cloud platforms, APIs, and other technologies.

From this exercise, developers will learn how to:

- Connect the Intel® Edison board or Intel® IoT Gateway, computing platforms designed for prototyping and producing IoT and wearable computing products.
- Interface with the Intel® Edison board or Arduino 101\* (branded Genuino 101\* outside the U.S.) board IO and sensor repository using MRAA and UPM from the Intel® IoT Developer Kit, a complete hardware and software solution to help developers explore the IoT and implement innovative projects.
- Run these code samples in the Intel® System Studio IoT Edition (Eclipse IDE for C/C++ and Java\* development) for creating applications that interact with sensors and actuators, enabling a quick start for developing software for the Intel® Edison board or the Intel® Galileo board.
- Set up a web application server to set the watering system time and store this data using Azure Redis Cache\* from Microsoft\*, Redis Store\* from IBM Bluemix\*, or Elasticache\* using Redis\* from Amazon Web Services (AWS)\*, different cloud services for connecting IoT solutions including data analysis, machine learning, and a variety of productivity tools to simplify the process of connecting your sensors to the cloud and getting your IoT project up and running quickly.
- Invoke the services of the Twilio\* API for sending text messages.
- Connect to a server using IoT Hub from Microsoft Azure\*, IoT from IBM Bluemix\*, IoT from Amazon Web Services (AWS)\*, AT&T M2X\*, Predix\* from GE, or SAP Cloud Platform\* IoT, different cloud-based IoT platforms for machine to machine communication.


## What it is

Using an Intel® Edison board or Intel® IoT Gateway, this project lets you create an automatic watering system that:

- turns a water pump on and off based on a configurable schedule.
- detects if the watering system is pumping when expected, by using a water flow sensor.
- can be accessed with your mobile phone via a built-in web interface to set the watering intervals.
- keeps track of the watering system, using cloud-based data storage.
- sends text messages to alert the user if the system if not working as expected.


## How it works

This watering system application has several useful features.

Using your mobile phone, you can set the watering system schedule using a web page served directly from the Intel® Edison board or Intel® IoT Gateway.

The watering system automatically checks the moisture sensor data at specified intervals and displays that data on the web page.

If the water pump is supposed to be on but the water flow sensor does not detect that the pumping is talking place as expected, the application sends an SMS to a specified number through Twilio\* so the watering system can be repaired.

Optionally, data can be stored using your own Microsoft Azure\*, IBM Bluemix\*, AT&T M2X\*, AWS\*, Predix\*, or SAP\* account.


## Hardware requirements

Grove\* Environment & Agriculture Kit containing:

1. Intel® Edison board with an Arduino\* breakout board
2. Grove Base Shield V2
3. [Grove Moisture Sensor](http://iotdk.intel.com/docs/master/upm/node/classes/grovemoisture.html)
4. [Water Pump](http://www.seeedstudio.com/depot/6V-Mini-Water-Pump-p-1945.html)
5. [Water Flow Sensor](http://iotdk.intel.com/docs/master/upm/node/classes/grovewfs.html)
6. [Dry-reed Relay](http://iotdk.intel.com/docs/master/upm/node/classes/groverelay.html)


## Software requirements

1. [Intel® System Studio (Eclipse IDE for C/C++ and Java\* development)](https://software.intel.com/en-us/node/672439)
2. Microsoft Azure\*, IBM Bluemix\*, AT&T M2X\*, AWS\*, Predix\*, or SAP\* account (optional)
3. Twilio\* account


### How to set up

This sample is already one of the IoT examples included in Intel® System Studio. To start using it, follow these steps:

1. From the menu bar, select **File > New > Intel Project for IoT**.<br>
![](./../../images/java/new_project.png)
2. Select the desired development board.<br>
![](./../../images/java/select_board.png)
3. Select the Operating System curently running on your development board.<br>
![](./../../images/java/select_OS.png)
4. Select **Intel® IoT Java Maven project**<br>
![](./../../images/java/select_language.png)
5. The next tab will ask for connection name and target name. If you do not know these click **Search Target**.<br>
![](./../../images/java/Target_connection.png)
6. From the list of available examples select **How to code Samples**, and select the sample you wish to import<br>
![](./../../images/java/select_sample.png)
7. Choose a name for your project and click **Finish**<br>
8. Your project source files will now be available on the on the upper left of your IDE by default.<br>
![](./../../images/java/project-src-imported.png)


### Connecting the Grove\* sensors

You need to have a Grove\* Base Shield V2 connected to an Arduino\* compatible breakout board to plug all the Grove devices into the Grove Base Shield V2. Make sure you have the tiny VCC switch on the Grove Base Shield V2 set to **5V**.

For this example, you need to power the Intel® Edison board with the external power adapter that comes with your starter kit, or else substitute an external **12V 1.5A** power supply. You can also use an external battery, such as a **5V** USB battery.

In addition, you need a breadboard and an extra **5V** power supply to provide power to the pump.

Note: You need a separate battery or power supply for the pump. You cannot use the same power supply for both the Intel® Edison board and the pump, so you need either 2 batteries or 2 power supplies.

You need to use the Grove\* Dry-reed Relay board to connect the Water Pump.

1. Plug one end of a Grove\* cable into the Grove\* Dry-reed Relay, and connect the other end to the D4 port on the Grove Base Shield V2.<br>
![](./../../images/js/watering.jpg)
2. Plug one wire from the Water Pump into the 5V power source reserved for the pump.
3. Plug the other wire from the Water Pump into one of the power connectors on the Grove Dry-reed Relay board.
4. Plug the other power connector on the Grove\* Dry-reed Relay board into the ground of the 5V power source reserved for the Water Pump.
5. Plug the Water Flow Sensor's red wire into the 5V pin on the Grove\* Base Shield V2.
6. Plug the Water Flow Sensor's black wire into the GND pin on the Grove\* Base Shield V2.
7. Plug the Water Flow Sensor's yellow wire into the 2 pin (aka digital pin 2) on the Grove\* Base Shield V2.
8. Plug one end of a Grove cable into the Grove\* Moisture Sensor, and connect the other end to the A0 port on the Grove Base Shield V2.


### Connecting the DFRobot\* sensors

![](./../../images/js/watering-dfrobot.jpg)

You need to have a I/O Expansion Shield connected to an Arduino\* compatible breakout board to plug all the DFRobot\* devices into the I/O Expansion Shield.

In addition, you need a breadboard and an extra 5V power supply to provide power to the pump. Note: you need a separate battery or power supply for the pump. You cannot use the same power supply for both the Intel® Edison board and the pump.

You need to use the Relay Module to connect the water pump.

1. Plug one end of a DFRobot\* cable into the Relay Module, and connect the other end to the A2 port on the I/O Expansion Shield.

2. Connect one wire from the pump to the GND of the power source reserved for the pump.

3. Connect the other wire from the pump to the NC (Normally Closed) connector on the Relay Module.

4. Connect the COM (Common) connector on the Relay Module to the + of the 5V power source reserved for the pump.

5. Plug one end of a DFRobot\* cable into the Moisture Sensor, and connect the other end to the A3 port on the I/O Expansion Shield.

### Copy the libraries on Windows\*

For help installing and using WinSCP, go to this link:

[using-winscp.md](./../../docs/cpp/using-winscp.md)

Note: You need to turn SSH on by running the `configure_edison --password` command on the board. Once you set the password, make sure you write it down. You only need to do this once and it is set when you reboot the Intel® Edison board.


### Twilio\* API Key

To optionally send text messages, you need to register for an account and get an API key from the Twilio\* website:

[https://www.twilio.com](https://www.twilio.com)

You can still run the example, but without a Twilio API key you cannot send SMS alerts.

### Configuring the example with your Twilio* API key

Pass your Twilio* API key and authentication token to the sample program by modifying the `TWILIO_ACCT_SID` and `TWILIO_AUTH_TOKEN` keys in the `config.properties` file. `NUMBER_TO_SEND_TO` and `TWILIO_OUTGOING_NUMBER` are also part of the Twilio* setup. For example:

```
  TWILIO_ACCT_SID="YOURAPIKEY"
  TWILIO_AUTH_TOKEN="YOURTOKEN"
  TWILIO_OUTGOING_NUMBER="YOURTWILIOOUTGOINGNUMBER"
  NUMBER_TO_SEND_TO="NUMBERTOSENDTO"
```

### IoT cloud setup

You can optionally store the data generated by this sample program using cloud-based IoT platforms from Microsoft Azure\*, IBM Bluemix\*, AT&T M2X\*, AWS\*, Predix\*, or SAP\*.

For information on how to connect to your own cloud server, go to:

[https://github.com/intel-iot-devkit/iot-samples-cloud-setup](https://github.com/intel-iot-devkit/iot-samples-cloud-setup)


### Data store server setup

Optionally, you can store the data generated by this sample program in a back-end database deployed using Microsoft Azure\*, IBM Bluemix\*, or AWS\*, along with Node.js\*, and a Redis\* data store.

For information on how to set up your own cloud data server, go to:

[https://github.com/intel-iot-devkit/intel-iot-examples-datastore](https://github.com/intel-iot-devkit/intel-iot-examples-datastore)

## Configuring the example for your hardware kit

To configure the example for the specific hardware kit that you are using, either Grove\* or DFRobot\* you will need to change the `INTEL_IOT_KIT` key in the `config.properties` file to either **GROVEKIT** (this is the default) or **DFROBOTKIT**, depending on which hardware kit you wish to use. For example:

```

  INTEL_IOT_KIT=GROVEKIT

```

### Connecting your Intel® Edison board or Intel® IoT Gateway to Intel® System Studio

1. On the **Target SSH Connections** tab, right-click your device and select **Connect**.<br>
![](./../../images/cpp/cpp-connection-eclipse-ide-win4.png)

If prompted for the username and password, the username is **root** and the password is whatever you specified when configuring the Intel® Edison board or Intel® IoT Gateway.


### Intel® IoT Gateway setup

You can run this example using an Intel® IoT Gateway connected to an Arduino 101\* (branded Genuino 101\* outside the U.S.).

Make sure your Intel® IoT Gateway is setup using Intel® IoT Gateway Software Suite, by following the directions on the web site here:

https://software.intel.com/en-us/getting-started-with-intel-iot-gateways-and-iotdk

The Arduino 101\* (branded Genuino 101\* outside the U.S.) needs to have the Firmata\* firmware installed. If you have IMRAA installed on your gateway, this will be done automatically. Otherwise, install the StandardFirmata or ConfigurableFirmata sketch manually onto your Arduino 101\* (branded Genuino 101\* outside the U.S.).


### IoT cloud setup

You can optionally store the data generated by this sample program using cloud-based IoT platforms from Microsoft Azure\*, IBM Bluemix\*, AT&T M2X\*, AWS\*, Predix\*, or SAP\*.

For information on how to connect to your own cloud server, go to:

[https://github.com/intel-iot-devkit/iot-samples-cloud-setup](https://github.com/intel-iot-devkit/iot-samples-cloud-setup)


### Data store server setup

Optionally, you can store the data generated by this sample program in a backend database deployed using Microsoft Azure\*, IBM Bluemix\*, or AWS\*, along with Node.js\*, and a Redis\* data store.

For information on how to set up your own cloud data server, go to:

[https://github.com/intel-iot-devkit/intel-iot-examples-datastore](https://github.com/intel-iot-devkit/intel-iot-examples-datastore)


### Running the example with the cloud server

To run the example with the optional back-end data store, you need to change the `SERVER` and `AUTH_TOKEN` keys in the `config.properties` file to the server URL and authentication token that correspond to your own data store server setup. For example:

```

  SERVER=http://intel-examples.azurewebsites.net/logger/watering-system
  AUTH_TOKEN="YOURTOKEN"

```


## Running the program using Intel® System Studio IoT Edition

When you're ready to run the example, make sure you have saved all the files.

Click the **Run** icon on the toolbar of Intel® System Studio IoT Edition. This runs the code on the Intel® Edison board or Intel® IoT Gateway.

![](./../../images/java/run_project.png)

You will see output similar to the following when the program is running.

![](./../../images/java/looks_when_running.png)


## Running the program from the command line

This can be easily achieved with basic Maven commands. For this to work you will need to have Maven installed, a guide can be found on the Maven website: <a href="https://maven.apache.org/install.html">https://maven.apache.org/install.html</a>

### Compiling on host machine and deploying to Intel® Edison board or Intel® IoT Gateway

If you want to compile the project on your local PC and then deploy it to the target you need to run `mvn package` at the location where the `pom.xml` file exists, or you can specify the file location using the `-f` parameter:

	$ mvn package -f <path_to_pom_file>

This will compile the source files and pack them in `.jar` archives. It will create a folder called `target` where you will find two jars, `WateringSystem-1.0-SNAPSHOT.jar` and `WateringSystem-1.0-SNAPSHOT-shaded.jar`. The first one contains only the classes from the current module, while the `shaded` version contains the classes from the current module and its dependencies, so running the program using the second jar will be easier since you don't have to worry about adding all the dependency jars to the classpath.

Next step is to copy the generated jar on the target using `scp`. The following command will copy the file to the `home` folder of user `root` on the Intel® Edison board or Intel® IoT Gateway:

	$ scp target/WateringSystem-1.0-SNAPSHOT-shaded.jar root@<target_ip>:

Then log in on the target using ssh:

	$ ssh root@<target_ip>

Next step is to run the program using `java`, providing the path to the copied jar file and the name of the `main` class:

	# java -cp WateringSystem-1.0-SNAPSHOT-shaded.jar howToCodeSamples.WateringSystem


### Running the program directly on the Intel® Edison board or Intel® IoT Gateway

If you have copied the source files from Git directly on the board and already installed Maven, then you can compile and run the program directly onto the target.
Log in to the board using ssh and navigate to the location of the `pom.xml` file.

First you will need to compile the source files:

	# mvn compile

Then you can execute the program. The following command will run the `main` file in a separate Java process:

	# mvn exec:exec
	


## Regenerating HTML and CSS

If you make any changes to either the **index.html** or **styles.css** file, you need to regenerate the .hex file used to serve up the assets via the built-in Crow\* web server.

For help using the shell script, go to this link:

[how-to-run-the-shellscript.md](./../../docs/cpp/how-to-run-the-shellscript.md)


## Setting the watering schedule

The schedule for the watering system is set using a single-page web interface served up from the Intel® Edison board or Intel® IoT Gateway while the sample program is running.<br>
![](./../../images/cpp/water-sys-app.png)

The latest data values from the connected Grove\* Moisture Sensor are displayed at the bottom of the web page.

The web server runs on port `3000`; if the Intel® Edison board or Intel® IoT Gateway is connected to Wi-Fi on `192.168.1.13`, the address to browse to if you are on the same network is `http://192.168.1.13:3000`.

IMPORTANT NOTICE: This software is sample software. It is not designed or intended for use in any medical, life-saving or life-sustaining systems, transportation systems, nuclear systems, or for any other mission-critical application in which the failure of the system could lead to critical injury or death. The software may not be fully tested and may contain bugs or errors; it may not be intended or suitable for commercial release. No regulatory approvals for the software have been obtained, and therefore software may not be certified for use in certain countries or environments.
