# Plant Lighting System

## Introduction

This automatic plant lighting system monitor application is part of a series of how-to Intel® IoT code sample exercises using the Intel® IoT Developer Kit, Intel® Edison development platform, cloud platforms, APIs, and other technologies.

From this exercise, developers will learn how to:
- Connect the Intel® Edison development platform, a computing platform designed for prototyping and producing IoT and wearable computing products.
- Interface with the Intel® Edison platform IO and sensor repository using MRAA and UPM from the Intel® IoT Developer Kit, a complete hardware and software solution to help developers explore the IoT and implement innovative projects.
- Run this code sample in Intel® XDK IoT Edition, an IDE for creating new applications that interact with sensors, actuators, and so on, enabling you to get a quick start on developing software for your Intel® Edison or Galileo board.
- Set up a web application server to store lighting system data using Azure Redis Cache* from Microsoft* Azure*, cloud services for connecting IoT solutions including data analysis, machine learning, and a variety of productivity tools to simplify the process of connecting your sensors to the cloud and getting your IoT project up and running quickly.
- Invoke the services of the Twilio* API for sending SMS messages.

## What It Is

Using an Intel® Edison board, this project lets you create a automatic plant lighting monitor system that:
- checks if a separate automated lighting system is turning on and off based on a configurable schedule, by using a light sensor;
- can be accessed with your mobile phone via the built-in web interface to set the lighting times;
- also monitors the water levels using a connected moisture sensor;
- logs events from the lighting system, using cloud-based data storage;
- sends SMS messages to alert if the system if not working as expected.

## How It Works

This lighting system has a few very useful features.
Set the lighting system schedule using a web page served directly from the Edison itself, using your mobile phone.

If the lighting is supposed to be on, but the light sensor does not detect light, then it sends an SMS alert to a specified number through Twilio.

It also automatically checks the moisture sensor data at periodic intervals, and logs that information.

Optionally, it can also store system events using the "Intel IoT Example Datastore" running on your own Microsoft Azure account.

## Hardware requirements

Grove* Indoor Environment Kit containing:

1. Intel® Edison with an Arduino* breakout board
2. [Grove* Moisture Sensor](http://iotdk.intel.com/docs/master/upm/node/classes/grovemoisture.html)
3. [Grove* Light Sensor](http://iotdk.intel.com/docs/master/upm/node/classes/grovelight.html)
4. [Grove* RGB LCD](http://iotdk.intel.com/docs/master/upm/node/classes/jhd1313m1.html)

## Software requirements

1. Eclipse*  IoT Edition
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

![](./../../../images/js/lighting.jpg)

You need to have a Grove* Shield connected to an Arduino-compatible breakout board, to plug in all the Grove* devices into the Grove* shield. Make sure you have the tiny VCC switch on the Grove* Shield set to "5V".

Plug one end of a Grove* cable into the "Grove* Light Sensor", and connect the other end to the "A0" port on the Grove* Shield.

Plug one end of a Grove* cable into the "Grove* Moisture Sensor", and connect the other end to the "A1" port on the Grove* Shield.

Plug one end of a Grove* cable into the "Grove* RGB LCD", and connect the other end to any of the "I2C" ports on the Grove* Shield.

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

## Regenerating the HTML and CSS

If you make any changes to either the `index.html` or `styles.css` files, you will need to regenerate the hex file used to serve up the assets on via the built-in Crow web server.
