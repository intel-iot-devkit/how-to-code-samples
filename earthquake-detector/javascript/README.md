# Earthquake Detector in JavaScript*

## What it is

Using a compatible Intel-based platform, this project lets you create an earthquake detector that:<br>
- senses motion using the digital accelerometer.<br>
- checks live earthquake data, using the USGS API.<br>
- displays the earthquake on the LCD.

## First time setup  
For all the samples in this repository, see the [General Setup Instructions](./../../README.md#setup) for requested boards and libraries. You need either Grove or DFRobot sensors but not both.

## Hardware requirements

You need to have a Grove\* Shield connected to an Arduino\*-compatible breakout board to plug all the Grove\* devices into the Grove\* Shield. Make sure you have the tiny VCC switch on the Grove\* Shield set to **5V**.

Sensor | Pin
--- | ---
Grove\* 3-Axis Digital Accelerometer | I2C
Grove\* RGB LCD | I2C

More details on the hardware requirements can be found in the [project README](./../README.md)

## Software requirements

1. [MRAA](https://github.com/intel-iot-devkit/mraa) and [UPM](https://upm.mraa.io) 
1. Node.js 
2. [MRAA and UPM Node.js bindings](https://github.com/intel-iot-devkit/upm/blob/master/docs/installing.md)
2. Packages as listed in the package.json file in this example 

## Configuring the example

To configure the example for the Grove\* kit, just leave the `kit` key in the `config.json` set to `grove`. To configure the example for the DFRobot\* kit, change the `kit` key in the `config.json` to `dfrobot` as follows:

```
{
  "kit": "dfrobot"
}
```

To configure the example for the Arduino\*/Genuino\* 101, add a `platform` key with the value `firmata` to the `config.json`, as follows:

```
{
  "kit": "grove",
  "platform": "firmata"
}
```

To configure the example to check for earthquakes in your area, change the `LATITUDE` and `LONGITUDE` keys in the `config.json` file as follows:

```
{
  "kit": "grove",
  "LATITUDE": "47.641944",
  "LONGITUDE": "-122.127222"
}
```

## Running the program 

To run this example on the board, simply enter

    $ npm install
    $ npm run
    $ ./earthquake-detector
    
You will see output similar to below when the program is running.

```
UPLOADING: Uploading project bundle to IoT device. 
[Upload Complete] Checking... 
No quake. 
```

Refer to [How it Works](./../README.md#how-it-works) for details on the functionality.


IMPORTANT NOTICE: This software is sample software. It is not designed or intended for use in any medical, life-saving or life-sustaining systems, transportation systems, nuclear systems, or for any other mission-critical application in which the failure of the system could lead to critical injury or death. The software may not be fully tested and may contain bugs or errors; it may not be intended or suitable for commercial release. No regulatory approvals for the software have been obtained, and therefore software may not be certified for use in certain countries or environments.
