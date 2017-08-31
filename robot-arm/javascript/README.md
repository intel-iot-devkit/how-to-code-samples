## What it is

Using a compatible Intel® IoT Platform, this project lets you create a robot arm that:<br>
- continuously checks the Grove\* Joystick.<br>
- moves two stepper motors based on the joystick control.<br>
- can be accessed via the built-in web interface to control the motors.

Additionally, the motors can be controlled individually via a web page served directly from the Intel® IoT Platform.

## First time setup  
For all the samples in this repository, see the ![General Setup Instructions](./../../README.md#setup) for required boards and libraries.

## Hardware requirements

### Grove\*

Sensor | Pin
--- | ---
Stepper motor controller #1 | Digital pins 4, 5, 6, and 7
Stepper motor controller #2 | Digital pins 9, 10, 11, and 12
Both controllers to ground (GND) | 5V power coming from the Arduino\* breakout board (VCC), and to the separate 5V power for the motors (VM).

More details on the hardware requirements can be found in the ![project README](./../README.md)

## Software requirements

1. Node.js 
2. MRAA and UPM Node.js bindings <link to mraa and upm install docs>
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

IMPORTANT NOTICE: This software is sample software. It is not designed or intended for use in any medical, life-saving or life-sustaining systems, transportation systems, nuclear systems, or for any other mission-critical application in which the failure of the system could lead to critical injury or death. The software may not be fully tested and may contain bugs or errors; it may not be intended or suitable for commercial release. No regulatory approvals for the software have been obtained, and therefore software may not be certified for use in certain countries or environments.
