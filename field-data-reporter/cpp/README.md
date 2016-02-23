# Wrist field data recorder

## Introduction

This wrist field data recorder application is part of a series of how-to Intel® IoT code sample exercises using the Intel® IoT Developer Kit, Intel® Edison development platform, cloud platforms, APIs, and other technologies.

From this exercise, developers will learn how to:
- Connect the Intel® Edison development platform, a computing platform designed for prototyping and producing IoT and wearable computing products.
- Interface with the Intel® Edison platform IO and sensor repository using MRAA and UPM from the Intel® IoT Developer Kit, a complete hardware and software solution to help developers explore the IoT and implement innovative projects.
- Run this code sample in Eclipse* IoT Edition, an IDE for creating new applications that interact with sensors, actuators, and so on, enabling you to get a quick start on developing software for your Intel® Edison or Galileo board.
- Set up a web application server to view data using a web page served up directly from the Intel® Edison.

## What it is

Using an Intel® Edison board, this project lets you create a wrist field data reporter that:
- continuously monitors the data read from the digital barometer;
- displays latest reading using the OLED display, when the touch sensor is tapped;
- serves up the recorded data in JSON format directly from the Intel® Edison.

## How it works

The wrist field data recorder that can be used to sample barometer data at regular intervals.

It also provides a way to look at the latest data captured, using the Q Touch Sensor and the OLED display.

These readings can be viewed or downloaded as JSON data served up directly from an embedded web server running on the Intel® Edison.

## Hardware requirements

Xadow* Starter Kit containing:

1. Intel® Edison with Xadow*-Edison Expansion Board
2. [Xadow - OLED display](http://iotdk.intel.com/docs/master/upm/node/classes/ssd1308.html)
3. [Xadow - Q Touch](http://iotdk.intel.com/docs/master/upm/node/classes/at42qt1070.html)
4. [Xadow - BMP180](http://iotdk.intel.com/docs/master/upm/node/classes/bmpx8x.html)

## Software requirements

1. [Eclipse*](https://software.intel.com/en-us/eclipse-getting-started-guide)

## How to set up

To begin, clone the **How-To Intel IoT Code Samples** repository with Git* on your computer as follows:

    $ git clone https://github.com/intel-iot-devkit/intel-iot-examples.git

Want to download a .zip file? In your web browser, go to [https://github.com/intel-iot-devkit/intel-iot-examples](https://github.com/intel-iot-devkit/intel-iot-examples) and click the **Download ZIP** button at the lower right. Once the .zip file is downloaded, uncompress it, and then use the files in the directory for this example.

### Adding the program to Eclipse*

In Eclipse*, select **Import Wizard** to import an existing project into the workspace as follows:

1. From the main menu, select **File > Import**.<br>
![](./../../../images/cpp/cpp-eclipse-menu.png)

2. The **Import Wizard** dialog box opens.
![](./../../../images/cpp/cpp-eclipse-menu-select-epiw.png)

3. Select **General > Existing Project into Workspace** and click **Next**.
![](./../../../images/cpp/cpp-eclipse-menue-epiw-rootdir.png)

4. Click **Select root directory** and then the associated **Browse** button to locate the directory that contains the project files.
![](./../../../images/cpp/cpp-eclipse-menu-select-rootdir.png)

5. Under **Projects**, select the directory with the project files you'd like to import.
![](./../../../images/cpp/cpp-eclipse-menue-epiw-rootdir.png)

6. Click **Finish** to import the files into Eclipse*.
![](./../../../images/cpp/cpp-eclipse-menu-src-loc.png)

Your main .cpp program is now displayed in your workspace under the **src** folder.

### Connecting The Xadow* Sensors

![](./../../../images/js/field-data.jpg)

You need to have a Xadow* expansion board connected to Intel® Edison to plug in all the Xadow* devices.

1. Plug one end of a Xadow* connector into the Xadow* OLED, and connect the other end to one of the side connectors on the Xadow* expansion board
2. Plug one end of a Xadow* connector into the Xadow* BMP180, and connect the other end to one of the side connectors on the Xadow* expansion board
3. Plug one end of a Xadow* connector into the Xadow Q Touch Sensor, and connect the other end to one of the other two connected devices.

### Intel® Edison setup

This example uses the Crow* web micro-framework to provide a simple-to-use, yet powerful web server. The **crow** library requires the **libboost** package be installed on Intel® Edison, as well as adding the needed include and lib files to the Eclipse* Cross G++ Compiler and Cross G++ Linker.

1. Update opkg base feeds so you can install the needed dependencies. Establish an SSH connection to Intel® Edison and run the following command:<br>
```
vi /etc/opkg/base-feeds.conf
```

2. Edit the file so that it contains the following:<br>
```
src/gz all http://repo.opkg.net/edison/repo/all
src/gz edison http://repo.opkg.net/edison/repo/edison
src/gz core2-32 http://repo.opkg.net/edison/repo/core2-32
```
3. Save the file by pressing **Esc**, then **:**, then **q**, and **Enter**.

This only needs to be done once per Intel® Edison board, so if you've already done it, you can skip to the next step.

Install the **boost** libraries onto Intel® Edison by running the following command:

```
opkg update
opkg install boost-dev
```

### Copy the libraries

You need to copy the libraries and include files from the board to your computer where you're running Eclipse* so the Cross G++ Compiler and Cross G++ Linker can find them. The easiest way to do this is by running the `scp` command from your computer (NOT Intel® Edison), as follows:

```
scp -r USERNAME@xxx.xxx.x.xxx:/usr/include/boost ~/Downloads/iotdk-ide-linux/devkit-x86/sysroots/i586-poky-linux/usr/include
scp USERNAME@xxx.xxx.x.xxx:/usr/lib/libboost* ~/Downloads/iotdk-ide-linux/devkit-x86/sysroots/i586-poky-linux/usr/lib
```

Change `USERNAME@xxx.xxx.x.xxx` to match whatever username and IP address you set your board to.

Change `~/Downloads/iotdk-ide-linux` to match the location on your computer where you installed the Intel® IoT Developer Kit.

### Copy the libraries on Windows*

We have a helpful link to get this set up here:

[https://github.com/hybridgroup/intel-iot-examples/blob/master/cpp/docs/using-winscp.md](https://github.com/hybridgroup/intel-iot-examples/blob/master/cpp/docs/using-winscp.md)

Note: you need to turn SSH on by running the `configure_edison --password` command on the board. Once you set the password, make sure you write it down. You only need to do this one time and it is set when you reboot Intel® Edison.

### Connecting your Intel® Edison to Eclipse*

![](./../../../images/cpp/cpp-connection-eclipse-ide-win.png)
1. In the bottom left corner, right-click anywhere in the **Target SSH Connections** tab and select **New > Connection**.<br> The **Intel(R) IoT Target Connection** window appears.

![](./../../../images/cpp/cpp-connection-eclipse-ide-win2.png)
2. In the **Filter** field, type the name of your board.

![](./../../../images/cpp/cpp-connection-eclipse-ide-win3.png)
3. In the **Select one of the found connections** list, select your device name and click **OK**.

![](./../../../images/cpp/cpp-connection-eclipse-ide-win4.png)
4. On the **Target SSH Connections** tab, right-click your device and select **Connect**.

If prompted for the username and password, the username is **root** and the password is whatever you specified when configuring Intel® Edison.

### Running the code on Intel® Edison

![](./../../../images/cpp/cpp-run-eclipse.png)

When you're ready to run the example, click **Run** at the top menu bar in Eclipse*. This compiles the program using the Cross G++ Compiler, links it using the Cross G++ Linker, transfers the binary to Intel® Edison, and then executes it on the board itself.

![](./../../../images/cpp/cpp-run-eclipse-successful-build.png)

After running the program, you should see output similar to the one in the image above.

![](./../../../images/cpp/field-data-oled.jpg)

When the program uploads to the Intel® Edison board your OLED display will show the current barometer reading, when you touch any of the Q-Touch buttons.

### Data access via browser

![](./../../../images/js/field-data-web.png)

The data collected can be downloaded directly via a webpage as JSON, served by the program running on the Intel® Edison.

The web server runs on port **3000**, so if the Intel® Edison is connected to WiFi on **192.168.1.13**, the address to browse to if you are on the same network is **http://192.168.1.13:3000**.
