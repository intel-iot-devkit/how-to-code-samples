# Watering System

## Introduction

This automatic watering system application is part of a series of how-to Intel®IoT code sample exercises using the Intel®IoT Developer Kit, Intel®Edison development platform, cloud platforms, APIs, and other technologies.

From this exercise, developers will learn how to:
- Connect the Intel®Edison development platform, a computing platform designed for prototyping and producing IoT and wearable computing products.
- Interface with the Intel®Edison platform IO and sensor repository using MRAA and UPM from the Intel®IoT Developer Kit, a complete hardware and software solution to help developers explore the IoT and implement innovative projects.
- Run this code sample in Eclipse* IoT Edition, an IDE for creating new applications that interact with sensors, actuators, and so on, enabling you to get a quick start on developing software for your Intel®Edison or Galileo board.
- Set up a web application server to store watering system data using Azure Redis Cache* from Microsoft* Azure*, cloud services for connecting IoT solutions including data analysis, machine learning, and a variety of productivity tools to simplify the process of connecting your sensors to the cloud and getting your IoT project up and running quickly.
- Invoke the services of the Twilio* API for sending SMS messages.

## What It Is

Using an Intel®Edison board, this project lets you create a automatic plant watering system that:
- turns on and off a water pump based on a configurable schedule;
- detects if watering system is pumping when expected, by using a water flow sensor;
- can be accessed with your mobile phone via the built-in web interface to set the watering times;
- keeps track of watering system, using cloud-based data storage;
- sends SMS messages to alert if the system if not working as expected.

## How It Works

This watering system has a few extremely useful features.
Set the watering system schedule using a web page served directly from the Edison itself, using your mobile phone.

It automatically checks the moisture sensor data at periodic intervals, and lets you see that data on the web page.

If the water pump is supposed to be on, but the water flow sensor does not detect that the pumping is talking place as expected, then it sends an SMS to a specified number through Twilio so the watering system can be repaired.

Optionally, it can also log watering system events using the "Intel IoT Example Datastore" running on your own Microsoft Azure account.

## Hardware requirements

Grove* Environment & Agriculture Kit containing:

1. IntelÂ® Edison with an Arduino* breakout board
2. [Grove* Moisture Sensor](http://iotdk.intel.com/docs/master/upm/node/classes/Grove*moisture.html)
3. [Water Pump](http://www.seeedstudio.com/depot/6V-Mini-Water-Pump-p-1945.html)
4. [Water Flow Sensor](http://www.seeedstudio.com/depot/G18-Water-Flow-Sensor-p-1346.html)
5. [Dry-reed Relay](http://iotdk.intel.com/docs/master/upm/node/classes/Grove*relay.html)

## Software requirements

1. [Eclipse* Iot version](https://software.intel.com/en-us/eclipse-getting-started-guide)
2. Microsoft* Azure* account
3. Twilio* account

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

![](./../../../images/js/watering.jpg)

You need to have a Grove* Shield connected to an Arduino-compatible breakout board, to plug in all the Grove* devices into the Grove* shield. Make sure you have the tiny VCC switch on the Grove* Shield set to "5V".

For this example, you will need to power the Intel Edison with the external power adaptor that came with your starter kit, or else substitute a external 12V 1.5A power supply. You can also use an external battery, such as a 5V USB battery.

In addition, you will need a breadboard and additional 5V power supply to provide power to the pump. Note that you need a separate battery or power supply for the pump. You cannot use the same power supply for both the Intel Edison and pump, so you will need either 2 batteries or 2 power supplies in total.

You will need to use the "Grove* Dry-reed Relay" board to connect the "Water Pump". Start by plugging one end of a Grove* cable into the "Grove* Dry-reed Relay", then connect the other end to the "D4" port on the Grove* Shield.

Connect one wire from the pump to the 5V power source reserved for the pump. Connect the other wire from the pump to one of the power connectors on the "Grove* Dry-reed Relay" board. Connect the other power connector on the "Grove* Dry-reed Relay" board to the ground of the 5V power source reserved for the pump.

Connect the "Water Flow Sensor" by connecting the red wire to the "5V" pin on the Grove* shield, the black wire to the GND pin on the Grove* Shield, then plug the yellow wire into the "2" pin (aka digital pin 2) on the Grove* Shield.

Plug one end of a Grove* cable into the "Grove* Moisture Sensor", and connect the other end to the "A0" port on the Grove* Shield.

### Intel Edison Setup

This example uses the `crow` web microframework library to provide a simple to use, yet powerful web server. The `crow` library requires the `libboost` package be installed on the Intel Edison, as well as adding the needed include and lib files to the Eclipse G++ Cross Compiler and G++ Cross Linker.

This example uses the `restclient-cpp` library to perform REST calls to the server. The code for `restclient-cpp` can be found in the `lib` directory. The `restclient-cpp` library requires the `libcurl` package, which is already installed on the Intel Edison by default.

It also uses the `twilio-cplusplus` library to communcate with the Twilio telephony service. The `twilio-cplusplus` library requires both the `libcurl` package and the `openssl` packages, which are already installed on the Intel Edison by default.

Update the opkg base feeds, so you can install the needed dependencies. Instructions on how to do this are located here: http://alextgalileo.altervista.org/edison-package-repo-configuration-instructions.html
If you've already done this, you can skip to the next step.

Now, install the boost libraries onto the Edison, by running:
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

### Twilio API Key

To optionally send SMS messages, you need to register for an account and get an API key from the Twilio web site: https://www.twilio.com/

You cannot send SMS messages without obtaining a Twilio API key first. You can still run the example, but without the SMS alerts.

Pass the `TWILIO_ACCT_SID` and `TWILIO_AUTH_TOKEN` to the example program, by modifying the `TWILIO_ACCT_SID` and `TWILIO_AUTH_TOKEN` keys in the run configuration menu -> commands to execute before application

```
  "TWILIO_ACCT_SID": "YOURAPIKEY"; "TWILIO_AUTH_TOKEN": "YOURTOKEN"
```

### Microsoft Azure server setup

Optionally, you can store the data generated by this example program in a backend database deployed using Microsoft Azure, Node.js, and a Redis data store.

For information on how to setup your own cloud data server, go to:

https://github.com/intel-iot-devkit/intel-iot-examples-datastore

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

## Configuring The Example

To configure the example for the optional SMS sending, obtain a key from the Twilio web site as documented above, to the example program, by modifying the `TWILIO_ACCT_SID` and `TWILIO_AUTH_TOKEN` keys in the run configuration menu -> commands to execute before application

```
  "TWILIO_ACCT_SID": "YOURAPIKEY"; "TWILIO_AUTH_TOKEN": "YOURTOKEN"
```

To configure the example for the optional Microsoft Azure data store, change the to the example program, by modifying the `TWILIO_ACCT_SID` and `TWILIO_AUTH_TOKEN` keys in the run configuration menu -> commands to execute before application
```
  "SERVER": "http://intel-examples.azurewebsites.net/logger/lighting-system"; "AUTH_TOKEN": "s3cr3t"
```

To configure the example for the both the the SMS and Microsoft Azure datastore, change the to the example program, by modifying the in the run configuration menu -> commands to execute before application `TWILIO_ACCT_SID`, `TWILIO_AUTH_TOKEN`, `SERVER` and `AUTH_TOKEN` keys as follows:

```
  "TWILIO_ACCT_SID": "YOURAPIKEY"; "TWILIO_AUTH_TOKEN": "YOURTOKEN"; "SERVER": "http://intel-examples.azurewebsites.net/logger/lighting-system"; "AUTH_TOKEN": "s3cr3t"
```
### Running The Code On Edison

When you're ready to run the example, you can click on the "Run" icon located in the menubar at the top of the Eclipse editor.
This will compile the program using the Cross G++ Compiler, link it using the Cross G++ Linker, transfer the binary to the Edison, and then execute it on the Edison itself.


### Running The Code On Edison

![](./../../../images/cpp/cpp-run-eclipse.png)

When you're ready to run the example, you can click on the "Run" icon located in the menubar at the top of the Eclipse editor.
This will compile the program using the Cross G++ Compiler, link it using the Cross G++ Linker, transfer the binary to the Edison, and then execute it on the Edison itself.

![](./../../../images/cpp/cpp-run-eclipse-successful-build.png)

After running the program you should have a similar output as in the image above.

When the program uploads to the Edison board your rgb lcd display will show the current time.

The web server runs on port 3000, so if the Intel® Edison board is connected to Wi-Fi* on 192.168.1.13, the address to browse to if you are on the same network is http://192.168.1.13:3000.
![](./../../../images/cpp/water-sys-app.png)
## Regenerating the HTML and CSS

If you make any changes to either the `index.html` or `styles.css` files, you will need to regenerate the hex file used to serve up the assets on via the built-in Crow web server.
We have a usefull tutorial on how to use the shell script here https://github.com/hybridgroup/intel-iot-examples/blob/master/cpp/docs/how-to-run-the-shellscript.md
