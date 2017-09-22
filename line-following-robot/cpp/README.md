# Line following robot in C++

## What it is

Using a compatible Intel® IoT Platform, this project lets you create a line-following robot that:

- continuously checks the line finder sensor.
- moves forward if on the line, using stepper motors.
- tries to pivot to find the line if not on it, using stepper motors.
- logs events from the line finder sensor, using cloud-based data storage.

## First time setup
For all the samples in this repository, see the ![General Setup Instructions](./../../README.md#setup) for required boards and libraries.  You need either Grove\* or DFRobot\* sensors but not both.

## Hardware requirements

### Grove\* 

Sensor | Pin
--- | ---
Stepper motor controller #1 | Digital pins 4, 5, 6, and 7
Stepper motor controller #2 | Digital pins 9, 10, 11, and 12
Both controllers to Ground (GND) | 5V power (VCC), and 5V power (VM)
Grove\* Line Finder | D2

For more specific information on the hardware requirements see ![Hardware Details](./../README.md#hardware-requirements)

## Software requirements

1. ![MRAA](https://github.com/intel-iot-devkit/mraa) and ![UPM](https://github.com/intel-iot-devkit/upm)
2. Microsoft Azure\*, IBM Bluemix\*, AT&T M2X\*, AWS\*, Predix\*, or SAP\* account (optional

Note: The following libraries are included with the repo and already linked to in the code -jsoncpp -restclient

## Running the program

To run this example on the board, simply enter

    $ make
    $ build
    $ ./line-following-robot

You will see output similar to below when the program is running.

```
{ value: '2016-04-22T03:28:35.962Z' } 
Connecting to MQTT server... 
moving forward 
MQTT message published: { d: { value: '2016-04-22T03:28:35.962Z' } } 
finding line 
```

## Running the example with the cloud server

To run the example with the optional backend data store, you need to set the `SERVER` and `AUTH_TOKEN` environment variables. You can do this in Intel® System Studio as follows:

1. From the **Run** menu, select **Run Configurations**.<br> The **Run Configurations** dialog box is displayed.
2. Under **C/C++ Remote Application**, click **doorbell**.<br> This displays the information for the application.
3. In the **Commands to execute before application** field, add the following environment variables, except use the server and authentication token that correspond to your own setup:<br>

        chmod 755 /tmp/line-follower; export SERVER="http://intel-examples.azurewebsites.net/logger/line-follower"; export AUTH_TOKEN="YOURTOKEN"

4. Click **Apply** to save your new environment variables.

Now when you run your program using the **Run** button, it should be able to call your server to save the data right from the Intel® IoT Platform.

IMPORTANT NOTICE: This software is sample software. It is not designed or intended for use in any medical, life-saving or life-sustaining systems, transportation systems, nuclear systems, or for any other mission-critical application in which the failure of the system could lead to critical injury or death. The software may not be fully tested and may contain bugs or errors; it may not be intended or suitable for commercial release. No regulatory approvals for the software have been obtained, and therefore software may not be certified for use in certain countries or environments.
