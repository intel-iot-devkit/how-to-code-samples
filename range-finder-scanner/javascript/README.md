# Range Finder Scanner in JavaScript*

## What it is

Using a compatible Intel-based platform, this project lets you create a range finding scanner that:<br>
- continuously checks the Grove\* IR Distance Interrupter.<br>
- moves the stepper motor in a 360-degree circle.<br>
- can be accessed via the built-in web interface to view range finder data.

## First time setup  
For all the samples in this repository, see the ![General Setup Instructions](./../../README.md#setup) for required boards and libraries. You need either Grove or DFRobot sensors but not both.

## Hardware requirements

### Grove\*

![](./images/range-finder-grove.JPG)

You need to have a Grove\* Shield connected to an Arduino\* compatible breakout board to plug all the Grove devices into the Grove Shield. Make sure you have the tiny VCC switch on the Grove Shield set to **5V**.

You need to power the Intel-based platform with the external power adaptor that comes with your starter kit, or substitute it with an external 12V 1.5A power supply. You can also use an external battery, such as a 5V USB battery.

In addition, you need a breadboard and an extra 5V power supply to provide power to the motor. Note: you need a separate battery or power supply for the motor. You cannot use the same power supply for both the Intel-based platform and the motor, so you need either 2 batteries or 2 power supplies in total.

Sensor | Pin
--- | ---
Stepper motor controller | Digital pins 9, 10, 11, and 12
Controller to ground (GND) | 5V (VCC) and 5V (VM)

Look at the bottom of the shield to determine the pin numbers.

More details on the hardware requirements can be found in the ![project README](./../README.md)

## Software requirements

1. [MRAA](https://github.com/intel-iot-devkit/mraa) and [UPM](https://upm.mraa.io) 
1. Node.js 
2. [MRAA and UPM Node.js bindings](https://github.com/intel-iot-devkit/upm/blob/master/docs/installing.md)
2. Packages as listed in the package.json file in this example 

## Configuring the example

To configure the example for the Intel® Edison board, just leave the `platform` key in the `config.json` set to `edison`. To configure the example for the Intel® IoT Gateway, change the `platform` key in the `config.json` to `firmata` as follows:

```
{
  "platform": "firmata"
}
```

## Running the program 

To run this example on the board, simply enter

    $ npm install
    $ npm run
    $ ./range-finder-scanner
  
You will see output similar to below when the program is running.

```
296 false
297 false
298 false
299 false 
```

Refer to ![How it Works](./../README.md#how-it-works) for details on the functionality.

IMPORTANT NOTICE: This software is sample software. It is not designed or intended for use in any medical, life-saving or life-sustaining systems, transportation systems, nuclear systems, or for any other mission-critical application in which the failure of the system could lead to critical injury or death. The software may not be fully tested and may contain bugs or errors; it may not be intended or suitable for commercial release. No regulatory approvals for the software have been obtained, and therefore software may not be certified for use in certain countries or environments.
