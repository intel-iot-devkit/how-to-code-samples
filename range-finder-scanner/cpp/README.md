# Range finder scanner in C++

## What It Is

Using a compatible Intel-based platform, this project lets you create a range finding scanner that:

- continuously checks the Grove\* IR Distance Interrupter.
- moves the stepper motor in a 360-degree circle.
- can be accessed via a built-in web interface to view range finder data.

## First time setup
For all the samples in this repository, see the ![General Setup Instructions](./../../README.md#setup) for required boards and libraries.  You need either Grove\* or DFRobot\* sensors but not both.

## Hardware requirements

### Grove\* 

Sensor | Pin
--- | ---
Stepper motor controller | Digital pins 9, 10, 11, and 12
Controller to ground (GND) | 5V (VCC) and 5V (VM)

For more specific information on the hardware requirements see ![Hardware Details](./../README.md#hardware-requirements)

## Software requirements

1. ![MRAA](https://github.com/intel-iot-devkit/mraa) and ![UPM](https://upm.mraa.io)
Note: The following libraries are included with the repo and already linked to in the code -jsoncpp -restclient
2. Intel® System Studio ![https://software.intel.com/en-us/creating-iot-projects-with-intel-system-studio-2018-c](https://software.intel.com/en-us/creating-iot-projects-with-intel-system-studio-2018-c) 

### Expected output
You will see output similar to below when the program is running.

```
296 false
297 false
298 false
299 false 
```

Range data is viewed using a single-page web interface served directly from your board while the sample program is running.<br>
![](./../../images/js/range-finder-web.png)

The web server runs on port `3000`, so if your board is connected to Wi-Fi on `192.168.1.13`, the address to browse to if you are on the same network is `http://192.168.1.13:3000`.

## Regenerating HTML and CSS

If you make any changes to either the **index.html** or **styles.css** file, you need to regenerate the .hex file used to serve up the assets via the built-in Crow\* web server.
For help using the shell script, go to this link:

[how-to-run-the-shellscript.md](./../../docs/cpp/how-to-run-the-shellscript.md)

IMPORTANT NOTICE: This software is sample software. It is not designed or intended for use in any medical, life-saving or life-sustaining systems, transportation systems, nuclear systems, or for any other mission-critical application in which the failure of the system could lead to critical injury or death. The software may not be fully tested and may contain bugs or errors; it may not be intended or suitable for commercial release. No regulatory approvals for the software have been obtained, and therefore software may not be certified for use in certain countries or environments.
