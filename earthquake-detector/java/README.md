# Earthquake detector in Java*

## Introduction

This earthquake detector application is part of a series of how-to Internet of Things (IoT) code sample exercises using the Intel® IoT Developer Kit, Intel® Edison board, Intel® IoT Gateway, cloud platforms, APIs, and other technologies.

From this exercise, developers will learn how to:

- Connect the Intel® Edison board or Intel® IoT Gateway, computing platforms designed for prototyping and producing IoT and wearable computing products.
- Interface with the Intel® or Arduino 101\* (branded Genuino 101\* outside the U.S.) board  IO and sensor repository using MRAA and UPM from the Intel® IoT Developer Kit, a complete hardware and software solution to help developers explore the IoT and implement innovative projects.
- Run these code samples in the Intel® System Studio IoT Edition (Eclipse IDE for C/C++ and Java\* development) for creating applications that interact with sensors and actuators, enabling a quick start for developing software for the Intel® Edison board or the Intel® Galileo board.
- Invoke the services of the United States Geological Survey (USGS) API for accessing earthquake data.


## What it is

Using an Intel® Edison board or Intel® IoT Gateway, this project lets you create an earthquake detector that:

- senses motion using a digital accelerometer.
- checks live earthquake data, using the USGS API.
- displays an earthquake warning on the LCD.


## How it works

This earthquake detector constantly reads the 3-axis digital accelerometer looking for movement that could indicate an earthquake.

When it thinks it detects an earthquake it uses the USGS API to try and verify if an earthquake actually occurred.

If the detector believes an earthquake did occur, it issues a warning using the LCD.


## Hardware requirements

Grove\* Starter Kit containing:

1. Intel® Edison board with an Arduino\* breakout board
2. Grove Base Shield V2
3. [Grove 3-Axis Digital Accelerometer](http://iotdk.intel.com/docs/master/upm/node/classes/mma7660.html)
4. [Grove RGB LCD](http://iotdk.intel.com/docs/master/upm/node/classes/jhd1313m1.html)

DFRobot\* Starter Kit for Intel® Edison board, containing:

1. Intel® Edison board with an Arduino\* breakout board
2. [Triple Axis Accelerometer](http://www.dfrobot.com/index.php?route=product/product&description=true&product_id=507).
3. [LCD Keypad Shield](http://iotdk.intel.com/docs/master/upm/node/classes/sainsmartks.html)

## Software requirements

1. [Intel® System Studio (Eclipse IDE for C/C++ and Java* development)](https://software.intel.com/en-us/node/672439)


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

You need to have the Grove\* Base Shield V2 connected to an Arduino\* compatible breakout board to plug all the Grove devices into the Grove Base Shield V2. Make sure you have the tiny VCC switch on the Grove Base Shield V2 set to **5V**.

1. Plug one end of a Grove cable into the Grove Accelerometer, and connect the other end to any of the I2C ports on the Grove Base Shield V2.<br>
![](./../../images/js/earthquake-detector.jpg)
2. Plug one end of a Grove cable into the Grove RGB LCD, and connect the other end to any of the I2C ports on the Grove Base Shield V2.


### Connecting the DFRobot\* sensors

![](./../../images/js/earthquake-detector-dfrobot.jpg)

You need to have a LCD Keypad Shield connected to an Arduino\* compatible breakout board to plug all the DFRobot\* devices into the LCD Keypad Shield.

1. Plug one end of a DFRobot\* cable into the plug labeled "X" on the Triple-Axis Accelerometer, then connect the other end to the A3 port on the LCD Keypad Shield.

2. Plug one end of a DFRobot\* cable into the plug labeled "Y" on the Triple-Axis Accelerometer, then connect the other end to the A2 port on the LCD Keypad Shield.

3. Plug one end of a DFRobot\* cable into the plug labeled "Z" on the Triple-Axis Accelerometer, then connect the other end to the A1 port on the LCD Keypad Shield.


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


## Configuring the example for your hardware kit

To configure the example for the specific hardware kit that you are using, either Grove\* or DFRobot\* you will need to change the `INTEL_IOT_KIT` key in the `config.properties` file to either **GROVEKIT** (this is the default) or **DFROBOTKIT**, depending on which hardware kit you wish to use. For example:

```

  INTEL_IOT_KIT=GROVEKIT

```


### Connecting your Intel® Edison board or Intel® IoT Gateway to Intel® System Studio

1. On the **Target SSH Connections** tab, right-click your device and select **Connect**.<br>
![](./../../images/cpp/cpp-connection-eclipse-ide-win4.png)

If prompted for the username and password, the username is **root** and the password is whatever you specified when configuring the Intel® Edison board or Intel® IoT Gateway.


## Running the program using Intel® System Studio IoT Edition

When you're ready to run the example, make sure you have saved all the files.

Click the **Run** icon on the toolbar of Intel® System Studio IoT Edition. This runs the code on the Intel® Edison board or Intel® IoT Gateway.

![](./../../images/java/run_project.png)

You will see output similar to the following when the program is running.

![](./../../images/java/looks_when_running.png)

When the the program loads correctly, your Grove\* RGB LCD shows the `quakebot ready` message.<br>
![](./../../images/cpp/earthquake-lcd.jpg)

If you shake the accelerometer, the detector checks to see if there really was an earthquake.


## Running the program from the command line

This can be easily achieved with basic Maven commands. For this to work you will need to have Maven installed, a guide can be found on the Maven website: <a href="https://maven.apache.org/install.html">https://maven.apache.org/install.html</a>

### Compiling on host machine and deploying to Intel® Edison board or Intel® IoT Gateway

If you want to compile the project on your local PC and then deploy it to the target you need to run `mvn package` at the location where the `pom.xml` file exists, or you can specify the file location using the `-f` parameter:

	$ mvn package -f <path_to_pom_file>

This will compile the source files and pack them in `.jar` archives. It will create a folder called `target` where you will find two jars, `EarthquakeDetector-1.0-SNAPSHOT.jar` and `EarthquakeDetector-1.0-SNAPSHOT-shaded.jar`. The first one contains only the classes from the current module, while the `shaded` version contains the classes from the current module and its dependencies, so running the program using the second jar will be easier since you don't have to worry about adding all the dependency jars to the classpath.

Next step is to copy the generated jar on the target using `scp`. The following command will copy the file to the `home` folder of user `root` on the Intel® Edison board or Intel® IoT Gateway:

	$ scp target/EarthquakeDetector-1.0-SNAPSHOT-shaded.jar root@<target_ip>:

Then log in on the target using ssh:

	$ ssh root@<target_ip>

Next step is to run the program using `java`, providing the path to the copied jar file and the name of the `main` class:

	# java -cp EarthquakeDetector-1.0-SNAPSHOT-shaded.jar howToCodeSamples.EarthquakeDetector


### Running the program directly on the Intel® Edison board or Intel® IoT Gateway

If you have copied the source files from Git directly on the board and already installed Maven, then you can compile and run the program directly onto the target.
Log in to the board using ssh and navigate to the location of the `pom.xml` file.

First you will need to compile the source files:

	# mvn compile

Then you can execute the program. The following command will run the `main` file in a separate Java process:

	# mvn exec:exec



IMPORTANT NOTICE: This software is sample software. It is not designed or intended for use in any medical, life-saving or life-sustaining systems, transportation systems, nuclear systems, or for any other mission-critical application in which the failure of the system could lead to critical injury or death. The software may not be fully tested and may contain bugs or errors; it may not be intended or suitable for commercial release. No regulatory approvals for the software have been obtained, and therefore software may not be certified for use in certain countries or environments.
