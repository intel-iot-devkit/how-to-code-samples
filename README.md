# Range Finder Scanner

This range finder scanner application is part of a series of how-to Intel® IoT code sample exercises using the Intel® IoT Developer Kit, Intel® Edison development platform, cloud platforms, APIs, and other technologies.

From this exercise, developers will learn how to:

- Connect the Intel® Edison development platform, a computing platform designed for prototyping and producing IoT and wearable computing products. Interface with the Intel® Edison platform IO and sensor repository using MRAA and UPM from the Intel® IoT Developer Kit, a complete hardware and software solution to help developers explore the IoT and implement innovative projects.
- Run this code sample in Eclipse* IoT Edition, an IDE for creating new applications that interact with sensors, actuators, and so on, enabling you to get a quick start on developing software for your Intel® Edison or Galileo board.
- Set up a web application server to view range finder data using a web page served up directly from the Intel® Edison.

## What It Is

Using an Intel® Edison board, this project lets you create a range finding scanner that:

- continuously checks the Grove* IR Distance Interrupter;
- moves the stepper motor in a 360 degree circle;
- can be accessed with via the built-in web interface to view range finder data.

## How It Works

As the stepper motor turns, it will pause to get readings from the Grove* IR Distance Interrupter.

These readings can be seen by viewing the web page served up directly from the Intel® Edison.

## Hardware requirements
Grove* Robotics Kit containing:

1. Intel® Edison with an Arduino* breakout board
2. [Grove* IR Distance Interrupter](http://iotdk.intel.com/docs/master/upm/node/classes/rfr359f.html)
3. [Stepper Motor Controller & Stepper Motor](http://iotdk.intel.com/docs/master/upm/node/classes/uln200xa.html)

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

### Connecting The Grove* Sensors

![](./../../../images/js/range-finder.jpg)

You will need to have the Grove Shield connected to the Arduino-compatible breakout board, in order to plug in all the various Grove devices into the Grove shield. Make sure you have the tiny VCC switch on the Grove Shield set to the "5V" position.

You will need to power the Intel Edison with the external power adaptor that came with your starter kit, or else substitute a external 12V 1.5A power supply. You can also use an external battery, such as a 5V USB battery.

In addition, you will need a breadboard and additional 5V power supply to provide power to the stepper motor. Note you need a separate battery or power supply for the motor. You cannot use the same power supply for both the Intel Edison and motor, so you will need either 2 batteries or 2 power supplies in total.

The stepper motor controller will need to be plugged into 4 pins on the Arduino breakout board, for it to be able to be controlled. It will need to be connected to pins 9, 10, 11, & 12. It also must be connected to ground (GND), to the 5V power coming from the Arduino breakout board (VCC), and to the separate 5V power for the motor (VM).

Plug one end of a Grove* cable into the "Grove* IR Distance Interrupter", and connect the other end to the "D2" port on the Grove* Shield.

### Intel® Edison Setup

This example uses the `crow` web microframework library to provide a simple to use, yet powerful web server. The `crow` library requires the `libboost` package be installed on the Intel® Edison, as well as adding the needed include and lib files to the Eclipse G++ Cross Compiler and G++ Cross Linker.

Update the opkg base feeds, so you can install the needed dependencies. SSH into the Intel® Edison, then run this command:

```
vi /etc/opkg/base-feeds.conf
```
Edit the file so that it contains the following:

```
src/gz all http://repo.opkg.net/edison/repo/all
src/gz edison http://repo.opkg.net/edison/repo/edison
src/gz core2-32 http://repo.opkg.net/edison/repo/core2-32
```
Now save the file, by pressing the "ESC" key, then the `:` key, then the `q` key, and hitting `ENTER`.

This only needs to be done once per Intel® Edison board, so if you've already done it, you can skip to the next step.

Now, install the boost libraries onto the Intel® Edison, by running:
```
opkg update
opkg install boost-dev
```

Now save the file, by pressing the `ESC` key, then the `:` key, then the `q` key, and hitting `ENTER`.

## Copy the Libraries
Next, you need to copy the libraries and include files from the Intel® Edison to your machine where you run Eclipse, so the G++ Cross Compiler and G++ Cross Linker can find them. The easiest way to do this is by running the `scp` command from your computer (NOT the Edison).

```
scp -r USERNAME@xxx.xxx.x.xxx:/usr/include/boost ~/Downloads/iotdk-ide-linux/devkit-x86/sysroots/i586-poky-linux/usr/include
scp USERNAME@xxx.xxx.x.xxx:/usr/lib/libboost* ~/Downloads/iotdk-ide-linux/devkit-x86/sysroots/i586-poky-linux/usr/lib
```
Change the `USERNAME@xxx.xxx.x.xxx` to match whatever username and IP address that you have set your Edison to.

Change `~/Downloads/iotdk-ide-linux` to match the location on your machine where you have installed the Eclipse IoT Development Kit.

## Copy the Libraries on Windows

We have a helpful link to get this set up here. https://github.com/hybridgroup/intel-iot-examples/blob/master/cpp/docs/using-winscp.md

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

## Regenerating the HTML and CSS

If you make any changes to either the `index.html` or `styles.css` files, you will need to regenerate the hex file used to serve up the assets on via the built-in Crow web server.
We have a useful tutorial on how to use the shell script here https://github.com/hybridgroup/intel-iot-examples/blob/master/cpp/docs/how-to-run-the-shellscript.md

### Web App Output

![](./../../../images/js/range-finder-web.png)

The web server runs on port 3000, so if the Intel® Edison board is connected to Wi-Fi* on 192.168.1.13, the address to browse to if you are on the same network is http://192.168.1.13:3000.
