# Earthquake detector in C++

## Introduction

This earthquake detector application is part of a series of how-to Intel® Internet of Things (IoT) code sample exercises using the Intel® IoT Developer Kit, Intel® Edison board, cloud platforms, APIs, and other technologies.

From this exercise, developers will learn how to:

- Connect the Intel® Edison board, a computing platform designed for prototyping and producing IoT and wearable computing products.
- Interface with the Intel® Edison board IO and sensor repository using MRAA and UPM from the Intel® IoT Developer Kit, a complete hardware and software solution to help developers explore the IoT and implement innovative projects.
- Run these code samples in the Intel® System Studio IoT Edition (Eclipse IDE for C/C++ and Java\* development) for creating applications that interact with sensors and actuators, enabling a quick start for developing software for the Intel® Edison board or the Intel® Galileo board.
- Invoke the services of the United States Geological Survey (USGS) API for accessing earthquake data.

## What it is

Using an Intel® Edison board, this project lets you create an earthquake detector that:

- senses motion using a digital accelerometer.
- checks live earthquake data, using the USGS API.
- displays an earthquake warning on the LCD.

## How it works

This earthquake detector constantly reads the 3-axis digital accelerometer looking for movement that could indicate an earthquake.

When it thinks it detects an earthquake it uses the USGS API to try and verify if an earthquake actually occurred.

If the detector believes an earthquake did occur, it issues a warning using the LCD.

## Hardware requirements

Grove* Starter Kit containing:

1. Intel® Edison board with an Arduino* breakout board
2. Grove Base Shield V2
3. [Grove 3-Axis Digital Accelerometer](http://iotdk.intel.com/docs/master/upm/node/classes/mma7660.html)
4. [Grove RGB LCD](http://iotdk.intel.com/docs/master/upm/node/classes/jhd1313m1.html)

## Software requirements

1. [Intel® System Studio (Eclipse IDE for C/C++ and Java* development)](https://software.intel.com/en-us/node/672439)

### How to set up

This sample is already one of the IoT examples included in Intel® System Studio. To start using it, follow these steps:

1. From the main menu, select **Intel® IoT > Import IoT Examples**.<br>
![](./../../images/cpp/import-iot-examples.png)
2. Expand the tree view for **C++ > How To Code Samples > Earthquake Detector** and click **Import Example**.<br>
![](./../../images/cpp/click-how-to-code-samples.png)
3. Select your developer board from the selection window then select **Next**.<br>
![](./../../images/cpp/select-board.png)
4. Select **Intel® IoT C/C++ project** from the Select a project type window then click **Next**.<br>
![](./../../images/cpp/select-project-type.png)
5. Select **Yocto** from the Selct target OS dropdown menu then click **Next**.<br>
![](./../../images/cpp/select-os.png)
6. The next tab will ask for connection name and target name. If you do not know these click **Search Target**.<br>
![](./../../images/cpp/search-target.png)
7. Select your Edison from the dropdown list. Select **OK**.<br>
![](./../../images/cpp/click-how-to-code-samples.png)
8. Your connection name and target name should be filled in. Select **Finish**.<br>
![](./../../images/cpp/finish-target.png)
9. A new window will open for you and you will need to choose a name for your project and click **Next**.<br>
![](./../../images/cpp/enter-project-name.png)
10. Your project source files will now be available on the on the upper left of your IDE by default.<br>
![](./../../images/cpp/project-src-imported.png)

### Connecting the Grove* sensors

You need to have the Grove* Base Shield V2 connected to an Arduino\*-compatible breakout board to plug all the Grove devices into the Grove Base Shield V2. Make sure you have the tiny VCC switch on the Grove Base Shield V2 set to **5V**.

1. Plug one end of a Grove cable into the Grove Accelerometer, and connect the other end to any of the I2C ports on the Grove Base Shield V2.<br>
![](./../../images/js/earthquake-detector.jpg)
2. Plug one end of a Grove cable into the Grove RGB LCD, and connect the other end to any of the I2C ports on the Grove Base Shield V2.

### Intel® Edison board setup

This example uses the **restclient-cpp** library to perform REST calls to the remote data server. The code can be found in the **lib** directory. The **restclient-cpp** library requires the **libcurl** package, which is already installed on the Intel® Edison board by default.

### Connecting your Intel® Edison board to Eclipse

1. On the **Target SSH Connections** tab, right-click your device and select **Connect**.<br>
![](./../../images/cpp/cpp-connection-eclipse-ide-win4.png)

If prompted for the username and password, the username is **root** and the password is whatever you specified when configuring the Intel® Edison board.

### Running the code on the Intel® Edison board

When you're ready to run the example, click **Run** at the top menu bar in Eclipse*.<br>
![](./../../images/cpp/cpp-run-eclipse.png)

This compiles the program using the Cross G++ Compiler, links it using the Cross G++ Linker, transfers the binary to the Intel® Edison board, and then executes it on the board itself.

After running the program, you should see output similar to the one in the image below.<br>
![](./../../images/cpp/cpp-run-eclipse-successful-build.png)

When the the program loads correctly, your Grove* RGB LCD shows the `quakebot ready` message.<br>
![](./../../images/cpp/earthquake-lcd.jpg)

If you shake the accelerometer, the detector checks to see if there really was an earthquake.

IMPORTANT NOTICE: This software is sample software. It is not designed or intended for use in any medical, life-saving or life-sustaining systems, transportation systems, nuclear systems, or for any other mission-critical application in which the failure of the system could lead to critical injury or death. The software may not be fully tested and may contain bugs or errors; it may not be intended or suitable for commercial release. No regulatory approvals for the software have been obtained, and therefore software may not be certified for use in certain countries or environments.
