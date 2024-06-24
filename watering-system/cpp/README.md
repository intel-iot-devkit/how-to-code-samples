# Watering system in C++

## What it is

Using a compatible Intel-based platform, this project lets you create an automatic watering system that:

- turns a water pump on and off based on a configurable schedule.
- detects if the watering system is pumping when expected, by using a water flow sensor.
- can be accessed with your mobile phone via a built-in web interface to set the watering intervals.
- keeps track of the watering system, using cloud-based data storage.
- sends text messages to alert the user if the system if not working as expected.

## First time setup
For all the samples in this repository, see the [General Setup Instructions](./../../README.md#setup) for required boards and libraries.  You need either Grove\* or DFRobot\* sensors but not both.

## Hardware requirements

### Grove\*

Sensor | Pin
--- | ---
Grove\* Dry-Reed Relay | D4
One wire from the pump | 5V power source
Other wire from pump | One of the Power connectors on the Grove\* Dry-Reed Relay board
Other power connector  on the Grove\* Dry-Reed Relay board | 5V power source reserved for the pump
Water Flow Sensor | Red wire into the 5V pin, the black wire into the GND pin, and the yellow wire into digital pin 2
Grove\* Moisture Sensor | A0

### DFRobot\*

Sensor | Pin
--- | ---
Relay Module | A1
One wire from the pump | GND of the power source
Other wire from the pump | NC (Normally Closed) connector on the Relay Module
COM (Common) connector | + of the 5V power source
Moisture Sensor | A3

For more specific information on the hardware requirements see ![Hardware Details](./../README.md#hardware-requirements)

## Software requirements

1. [MRAA](https://github.com/intel-iot-devkit/mraa) and [UPM](https://upm.mraa.io)
2. [Intel® System Studio](https://software.intel.com/en-us/creating-iot-projects-with-intel-system-studio-2018-c) 
3. Microsoft Azure\*, IBM Bluemix\*, AT&T M2X\*, AWS\*, Predix\*, or SAP\* account (optional)
4. Twilio\* account (optional)

Note: The following libraries are included with the repo and already linked to in the code -jsoncpp -restclient

### Twilio\* API Key (optional)

To optionally send text messages, you need to register for an account and get an API key from the Twilio\* website:

[https://www.twilio.com](https://www.twilio.com)

You can still run the example, but without a Twilio API key you cannot send SMS alerts.

## Expected output
    
You will see output similar to below when the program is running.

```
UPLOADING: Uploading project bundle to IoT device. 
[Upload Complete] moisture (46) 
Connecting to MQTT server... 
MQTT message published: { d: { value: 'moisture (46) 2016-04-22705:14:56.681Z' } } 
```
## Setting the watering schedule

The schedule for the watering system is set using a single-page web interface served up from the Intel® Edison board or Intel® IoT Gateway while the sample program is running.<br>
![](./../../images/cpp/water-sys-app.png)

The latest data values from the connected Grove\* Moisture Sensor are displayed at the bottom of the web page.

The web server runs on port `3000`; if the your board is connected to Wi-Fi on `192.168.1.13`, the address to browse to if you are on the same network is `http://192.168.1.13:3000`.
Refer to ![How it Works](./../README.md#how-it-works) for details on the functionality.

## Running the example with the cloud server (optional)

To run the example with the optional backend data store, you need to set the `SERVER` and `AUTH_TOKEN` environment variables. You can do this in Intel® System Studio as follows:

1. From the **Run** menu, select **Run Configurations**.<br> The **Run Configurations** dialog box is displayed.
2. Under **C/C++ Remote Application**, click **watering-system**.<br> This displays the information for the application.
3. In the **Commands to execute before application** field, add the following environment variables, except use the server and authentication token that correspond to your own setup:<br>

        chmod 755 /tmp/watering-system; export SERVER="http://intel-iot-example-data.azurewebsites.net/logger/watering-system"; export AUTH_TOKEN="Enter Auth Token Here"; export TWILIO_SID="Enter Twilio SID Here"; export TWILIO_TOKEN="Enter Twilio Token Here"; export TWILIO_TO="Enter Number to Send to here Formattted 555-555-5555"; export TWILIO_FROM="Enter Number to be Sent From Here Formated 555-555-5555"

4. Click **Apply** to save your new environment variables.

Now when you run your program using the **Run** button, it should be able to call your server to save the data right from your board.

## Regenerating HTML and CSS

If you make any changes to either the **index.html** or **styles.css** file, you need to regenerate the .hex file used to serve up the assets via the built-in Crow\* web server.

For help using the shell script, go to this link:

[how-to-run-the-shellscript.md](./../../docs/cpp/how-to-run-the-shellscript.md)

IMPORTANT NOTICE: This software is sample software. It is not designed or intended for use in any medical, life-saving or life-sustaining systems, transportation systems, nuclear systems, or for any other mission-critical application in which the failure of the system could lead to critical injury or death. The software may not be fully tested and may contain bugs or errors; it may not be intended or suitable for commercial release. No regulatory approvals for the software have been obtained, and therefore software may not be certified for use in certain countries or environments.
