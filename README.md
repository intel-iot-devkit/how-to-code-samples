# Wrist Field Data Recorder

## Introduction

This wrist field data recorder application is part of a series of how-to Intel® IoT code sample exercises using the Intel® IoT Developer Kit, Intel® Edison development platform, cloud platforms, APIs, and other technologies.

From this exercise, developers will learn how to:
- Connect the Intel® Edison development platform, a computing platform designed for prototyping and producing IoT and wearable computing products.
- Interface with the Intel® Edison platform IO and sensor repository using MRAA and UPM from the Intel® IoT Developer Kit, a complete hardware and software solution to help developers explore the IoT and implement innovative projects.
- Run this code sample in Eclipse* IoT Edition, an IDE for creating new applications that interact with sensors, actuators, and so on, enabling you to get a quick start on developing software for your Intel® Edison or Galileo board.
- Set up a web application server to view data using a web page served up directly from the Intel® Edison.

## What It Is

Using an Intel® Edison board, this project lets you create a wrist field data reporter that:
- continuously monitors the data read from the digital barometer;
- displays latest reading using the OLED display, when the touch sensor is tapped;
- serves up the recorded data in JSON format directly from the Intel® Edison.

## How It Works

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

1. [Eclipse* Iot version](https://software.intel.com/en-us/eclipse-getting-started-guide)

### How To Setup

To begin, clone the Intel® IoT Examples repository with Git on your computer as follows:

    $ git clone https://github.com/intel-iot-devkit/intel-iot-examples.git

Just want to download a ZIP file? Just point your web browser to the Github repo at [https://github.com/intel-iot-devkit/intel-iot-examples](https://github.com/intel-iot-devkit/intel-iot-examples) and click on the "Download ZIP" button at the lower right. Once the ZIP file has finished downloading, uncompress it, and then use the files in the directory for this example.

## Adding The Code To Eclipse IoT

You use the Eclipse "Import Wizard" to import an existing project into the workspace as follows:

- From the main menu bar, select "File > Import..."
![](./../../../images/cpp/cpp-eclipse-menu.png)

- The "Import wizard" dialog will open.
![](./../../../images/cpp/cpp-eclipse-menu-select-epiw.png)

- Select "General > Existing Project into Workspace" and click on the "Next" button.
![](./../../../images/cpp/cpp-eclipse-menue-epiw-rootdir.png)

- Choose "Select root directory", then click on the associated "Browse" button to locate the directory that contains the project files.
![](./../../../images/cpp/cpp-eclipse-menu-select-rootdir.png)

- Under "Projects" select the directory with the project files which you would like to import.
![](./../../../images/cpp/cpp-eclipse-menue-epiw-rootdir.png)
- Click on the "Finish" button to import the files into Eclipse.

![](./../../../images/cpp/cpp-eclipse-menu-src-loc.png)
- Your main .cpp program will now be in your workspace under the src folder.

### Connecting The Xadow* Sensors

![](./../../../images/js/field-data.jpg)

You will need to have the Xadow* expansion board connected to the Intel® Edison, in order to plug in all the various Xadow* devices.

For more information on how to set up this expansion board, see [Seeed's wiki page](http://www.seeedstudio.com/wiki/Xadow_-_Edison).

Plug one end of a Xadow* connector into the "Xadow* - OLED", and connect the other end to one of the side connectors on the Xadow* expansion board

Plug one end of a Xadow* connector into the "Xadow* - BMP180", and connect the other end to one of the side connectors on the Xadow* expansion board

Plug one end of a Xadow* connector into the "Xadow - Q Touch Sensor", and connect the other end to one of the other two connected devices.

### Intel Edison Setup

This example uses the `crow` web microframework library to provide a simple to use, yet powerful web server. The `crow` library requires the `libboost` package be installed on the Intel Edison, as well as adding the needed include and lib files to the Eclipse G++ Cross Compiler and G++ Cross Linker.

Update the opkg base feeds, so you can install the needed dependencies. SSH into the Intel Edison, then run this command:

```
vi /etc/opkg/base-feeds.conf
```

Edit the file so that it contains the following:

```
src/gz all http://repo.opkg.net/edison/repo/all
src/gz edison http://repo.opkg.net/edison/repo/edison
src/gz core2-32 http://repo.opkg.net/edison/repo/core2-32
```

Now save the file, by pressing the `ESC` key, then the `:` key, then the `q` key, and hitting `ENTER`.

This only needs to be done once per Intel Edison board, so if you've already done it, you can skip to the next step.

Now, install the boost libraries onto the Intel Edison, by running:

```
opkg update
opkg install boost-dev
```

## Copy the Libraries
Next, you need to copy the libraries and include files from the Edison to your machine where you run Eclipse, so the G++ Cross Compiler and G++ Cross Linker can find them. The easiest way to do this is by running the `scp` command from your computer (NOT the Edison).

```
scp -r USERNAME@xxx.xxx.x.xxx:/usr/include/boost ~/Downloads/iotdk-ide-linux/devkit-x86/sysroots/i586-poky-linux/usr/include
scp USERNAME@xxx.xxx.x.xxx:/usr/lib/libboost* ~/Downloads/iotdk-ide-linux/devkit-x86/sysroots/i586-poky-linux/usr/lib
```

Change the `USERNAME@xxx.xxx.x.xxx` to match whatever username and IP address that you have set your Edison to.

Change `~/Downloads/iotdk-ide-linux` to match the location on your machine where you have installed the Eclipse IoT Development Kit.

## Copy the Libraries on Windows

We have a helpful link to get this set up here. https://github.com/hybridgroup/intel-iot-examples/blob/master/cpp/docs/using-winscp.md

Note that you will need to have turned on SSH by running the `configure_edison --password` command on the Edison. Once you've set the password, make sure you write it down. You only need to do this one time and it will be set when you reboot your Edison.

You will also want to set the current time zone to match the zone you are in. You do this using the `timedatectl` program on the Edison itself. For example:

```
timedatectl set-timezone America/Los_Angeles
```

### Connecting Your Edison to the Eclipse IDE

![](./../../../images/cpp/cpp-connection-eclipse-ide-win.png)
1. In the bottom left corner right-click in the area "Target SSH Conections" select "New..." then select "Connection..." and a new screen will appear.

![](./../../../images/cpp/cpp-connection-eclipse-ide-win2.png)
2. In the "filter box" type the name of your edison. In the example mine is JustinEdison.

![](./../../../images/cpp/cpp-connection-eclipse-ide-win3.png)
3. In the "Select one of the found connections list; click on your device name. Then Ok.

![](./../../../images/cpp/cpp-connection-eclipse-ide-win4.png)
4. Your device will now appear in the "Target SSH Connections" area. Right-clickt it and select connect.
(If promted for a username and password the user is 'root' and password is whatever you set it up as when configuring the Edison board)

### Running The Code On Edison

![](./../../../images/cpp/cpp-run-eclipse.png)

When you're ready to run the example, you can click on the "Run" icon located in the menubar at the top of the Eclipse editor.
This will compile the program using the Cross G++ Compiler, link it using the Cross G++ Linker, transfer the binary to the Edison, and then execute it on the Edison itself.

![](./../../../images/cpp/cpp-run-eclipse-successful-build.png)

After running the program you should have a similar output as in the image above.

![](./../../../images/cpp/field-data-oled.jpg)

When the program uploads to the Edison board your OLED display will show the current barometer reading, when you touch any of the Q-Touch buttons.

### Data Access via Browser

![](./../../../images/js/field-data-web.png)

The data collected can be downloaded directly via a webpage as JSON, served by the program running on the Edison.

The web server runs on the Intel Edison's port 3000, so if the Intel Edison is connected to WiFi on `192.168.1.13`, the address to browse to if you are on the same network is `http://192.168.1.13:3000`.
