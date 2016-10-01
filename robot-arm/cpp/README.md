# Robot arm in C++

## Introduction

This robot arm application is part of a series of how-to Intel® Internet of Things (IoT) code sample exercises using the Intel® IoT Developer Kit, Intel® Edison board, cloud platforms, APIs, and other technologies.

From this exercise, developers will learn how to:

- Connect the Intel® Edison board, a computing platform designed for prototyping and producing IoT and wearable computing products.
- Interface with the Intel® Edison board IO and sensor repository using MRAA and UPM from the Intel® IoT Developer Kit, a complete hardware and software solution to help developers explore the IoT and implement innovative projects.
- Run these code samples in the Intel® System Studio IoT Edition (Eclipse IDE for C/C++ and Java\* development) for creating applications that interact with sensors and actuators, enabling a quick start for developing software for the Intel® Edison board or the Intel® Galileo board.
- Set up a web application server to control a robot arm using a web page served up directly from the Intel® Edison board.

## What it is

Using an Intel® Edison board, this project lets you create a robot arm that:

- continuously checks the Grove* Joystick.
- moves two stepper motors, based on the control of the joystick.
- can be accessed via a built-in web interface to control the motors.

## How it works

This example allows you to control a robotic arm using a thumb joystick. Each axis of the joystick corresponds one of the two stepper motors.

Additionally, the motors can be controlled individually via a web page served up directly from the Intel® Edison board.

## Hardware requirements

Grove* Robotics Kit containing:

1. Intel® Edison board with an Arduino* breakout board
2. Grove Base Shield V2
3. [Grove Thumb Joystick](http://iotdk.intel.com/docs/master/upm/node/classes/joystick12.html)
4. [Stepper Motor Controller & Stepper Motor](http://iotdk.intel.com/docs/master/upm/node/classes/uln200xa.html) (x2)

## Software requirements

1. [Intel® System Studio IoT Edition (Eclipse IDE for C/C++ and Java* development)](https://software.intel.com/en-us/eclipse-getting-started-guide)

### How to set up

This sample is already one of the IoT examples included in Intel® System Studio. To start using it, follow these steps:

1. From the main menu, select **Intel® IoT > Import IoT Examples**.<br>
![](./../../images/cpp/import-iot-examples.png)
2. The **Intel System Studio IoT Edition Examples** tab opens. Click the offline button so it becomes online.<br>
![](./../../images/cpp/click-offline.png)
3. Expand the tree view for **C++ > How To Code Samples > Robot Arm** and click **Import Example**.<br>
![](./../../images/cpp/click-how-to-code-samples.png)
4. A new window will open for you and you will need to choose a name for your project and click **Next**.<br>
![](./../../images/cpp/enter-project-name.png)
5. The next tab will ask for connection name and target name. If you do not know these click **Search Target**.<br>
![](./../../images/cpp/search-target.png)
6. Select your Edison from the dropdown list. Select **OK**.<br>
![](./../../images/cpp/click-how-to-code-samples.png)
7. Your connection name and target name should be filled in. Select **Finish**.<br>
![](./../../images/cpp/finish-target.png)
8. Your project source files will now be available on the on the upper left of your IDE by default.<br>
![](./../../images/cpp/project-src-imported.png)

### Connecting the Grove* sensors

You need to have a Grove* Base Shield V2 connected to the Arduino\*-compatible breakout board to plug all the Grove devices into the Grove Base Shield V2. Make sure you have the tiny VCC switch on the Grove Base Shield V2 set to **5V**.

You need to power the Intel® Edison board with the external power adapter that comes with your starter kit, or substitute an external **12V 1.5A** power supply. You can also use an external battery, such as a **5V** USB battery.

In addition, you need a breadboard and an extra **5V** power supply to provide power to both the motors. 

Note: You need a separate battery or power supply for the motors. You cannot use the same power supply for both the Intel® Edison board and the motors, so you need either 2 batteries or 2 power supplies in total.

1. Plug each stepper motor controller into four pins on the Arduino* breakout board. Connect stepper motor controller #1 to pins 4, 5, 6, and 7, and stepper motor controller #2 to pins 8, 9, 10, and 11. Connect both the controllers to ground (GND), to the **5V** power coming from the Arduino* breakout board (VCC), and to the separate **5V** power for the motors (VM).<br>
![](./../../images/js/robot-arm.jpg)
2. Plug one end of a Grove* cable into the Grove Joystick, and connect the other end to the A0 port on the Grove Base Shield V2.

### Intel® Edison board setup

This example uses the Crow web micro-framework to provide a simple-to-use, yet powerful web server. The Crow library requires the **libboost** package be installed on the Intel® Edison board, as well as adding the needed include and lib files to the Eclipse Cross G++ Compiler and Cross G++ Linker.

1. Update opkg base feeds so you can install the needed dependencies. Establish an SSH connection to the Intel® Edison board and run the following command:<br>

        vi /etc/opkg/base-feeds.conf

2. Edit the file so that it contains the following:<br>

        src/gz all http://repo.opkg.net/edison/repo/all
        src/gz edison http://repo.opkg.net/edison/repo/edison
        src/gz core2-32 http://repo.opkg.net/edison/repo/core2-32

3. Save the file by pressing **Esc**, then **:**, then **q**, and **Enter**.

This only needs to be done once per Intel® Edison board; if you've already done it, you can skip to the next step.

Install the **boost** libraries onto the Intel® Edison board by running the following command:

    opkg update
    opkg install boost-dev

### Copy the libraries

You need to copy the libraries and include files from the board to your computer where you're running Eclipse so the Cross G++ Compiler and Cross G++ Linker can find them. The easiest way to do this is by running the `scp` command from your computer (NOT the Intel® Edison board), as follows:

    scp -r USERNAME@xxx.xxx.x.xxx:/usr/include/boost ~/Downloads/iotdk-ide-linux/devkit-x86/sysroots/i586-poky-linux/usr/include
    scp USERNAME@xxx.xxx.x.xxx:/usr/lib/libboost* ~/Downloads/iotdk-ide-linux/devkit-x86/sysroots/i586-poky-linux/usr/lib

Change `USERNAME@xxx.xxx.x.xxx` to match whatever username and IP address you set your board to.

Change `~/Downloads/iotdk-ide-linux` to match the location on your computer where you installed the Intel® IoT Developer Kit.

### Copy the libraries on Windows*

For help using the shell script, go to this link: 

[using-winscp.md](./../../docs/cpp/using-winscp.md)

Note: You need to turn SSH on by running the `configure_edison --password` command on the board. Once you set the password, make sure you write it down. You only need to do this one time and it is set when you reboot the Intel® Edison board.

### Connecting your Intel® Edison board to Eclipse

1. In the bottom left corner, right-click anywhere on the **Target SSH Connections** tab and select **New > Connection**.<br>
![](./../../images/cpp/cpp-connection-eclipse-ide-win.png)
2. The **Intel(R) IoT Target Connection** window appears. In the **Filter** field, type the name of your board.<br>
![](./../../images/cpp/cpp-connection-eclipse-ide-win2.png)
3. In the **Select one of the found connections** list, select your device name and click **OK**.<br>
![](./../../images/cpp/cpp-connection-eclipse-ide-win3.png)
4. On the **Target SSH Connections** tab, right-click your device and select **Connect**.<br>
![](./../../images/cpp/cpp-connection-eclipse-ide-win4.png)

If prompted for the username and password, the username is **root** and the password is whatever you specified when configuring the Intel® Edison board.

### Running the code on the Intel® Edison board

When you're ready to run the example, click **Run** at the top menu bar in Eclipse.<br>
![](./../../images/cpp/cpp-run-eclipse.png)

This compiles the program using the Cross G++ Compiler, links it using the Cross G++ Linker, transfers the binary to the Intel® Edison board, and then executes it on the board itself.

After running the program, you should see output similar to the one in the image below.<br>
![](./../../images/cpp/cpp-run-eclipse-successful-build.png)

## Regenerating HTML and CSS

If you make any changes to either the **index.html** or **styles.css** file, you need to regenerate the .hex file used to serve up the assets via the built-in Crow* web server.

For help using the shell script, go to this link: 

[how-to-run-the-shellscript.md](./../../docs/cpp/how-to-run-the-shellscript.md)

### Controlling the robot arm via a web browser

The robot arm is controlled using a single-page web interface served directly from the Intel® Edison board while the sample program is running.<br>
![](./../../images/cpp/robot-arm-web.png)

The web server runs on port `3000`; if the Intel® Edison board is connected to Wi-Fi* on `192.168.1.13`, the address to browse to if you are on the same network is `http://192.168.1.13:3000`.

IMPORTANT NOTICE: This software is sample software. It is not designed or intended for use in any medical, life-saving or life-sustaining systems, transportation systems, nuclear systems, or for any other mission-critical application in which the failure of the system could lead to critical injury or death. The software may not be fully tested and may contain bugs or errors; it may not be intended or suitable for commercial release. No regulatory approvals for the software have been obtained, and therefore software may not be certified for use in certain countries or environments.
