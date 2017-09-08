## What it is

Using a compatible Intel® IoT Platform, this project lets you create an automatic watering system that:<br>
- turns a water pump on or off based on a configurable schedule.<br>
- detects if the pumping occurs when expected, by using a water flow sensor.<br>
- can be accessed with your mobile phone via the built-in web interface to set the watering times;<br>
- keeps track of watering events, using cloud-based data storage.<br>
- sends text messages to alert recipients if the system is not working as expected.

## First time setup  
For all the samples in this repository, see the ![General Setup Instructions](./../../README.md#setup) for required boards and libraries. You need either Grove or DFRobot sensors but not both.

## Hardware requirements

### Grove\*

You need to have a Grove\* Shield connected to an Arduino\*-compatible breakout board to plug all the Grove\* devices into the Grove\* Shield. Make sure you have the tiny VCC switch on the Grove\* Shield set to **5V**.

You need to power the Intel® IoT Platform with the external power adapter that comes with your starter kit, or substitute it with an external 12V 1.5A power supply. You can also use an external battery, such as a 5V USB battery.

In addition, you need a breadboard and an extra 5V power supply to provide power to the pump. Note: you need a separate battery or power supply for the pump. You cannot use the same power supply for both the Intel® IoT Platform and the pump, so you need either 2 batteries or two power supplies in total.

You need to use the Grove\* Dry-Reed Relay board to connect the water pump.

Sensor | Pin
--- | ---
Grove\* Dry-Reed Relay | D4
One wire from the pump | 5V power source
Other wire from pump | One of the Power connectors on the Grove\* Dry-Reed Relay board
Other power connector  on the Grove\* Dry-Reed Relay board | 5V power source reserved for the pump
Water Flow Sensor | Red wire into the 5V pin, the black wire into the GND pin, and the yellow wire into digital pin 2
Grove\* Moisture Sensor | A0

### DFRobot\*

You need to have a I/O Expansion Shield connected to an Arduino\* compatible breakout board to plug all the DFRobot\* devices into the I/O Expansion Shield.

In addition, you need a breadboard and an extra 5V power supply to provide power to the pump. Note: you need a separate battery or power supply for the pump. You cannot use the same power supply for both the Intel® IoT Platform and the pump.

You need to use the Relay Module to connect the water pump.

Sensor | Pin
--- | ---
Relay Module | A1
One wire from the pump | GND of the power source
Other wire from the pump | NC (Normally Closed) connector on the Relay Module
COM (Common) connector | + of the 5V power source
Moisture Sensor | A3

More details on the hardware requirements can be found in the ![project README](./../README.md)

## Software requirements

1. ![MRAA](https://github.com/intel-iot-devkit/mraa) and ![UPM](https://github.com/intel-iot-devkit/upm) 
1. Node.js 
2. MRAA and UPM Node.js bindings <link to mraa and upm install docs>
2. Packages as listed in the package.json file in this example 
2. Microsoft Azure\*, IBM Bluemix\*, AT&T M2X\*, AWS\*, GE Predix\*, or SAP\* account (optional)
3. Twilio\* account (optional)

### Twilio\* API key

To optionally send text messages, you need to register for an account and get an API key from the Twilio\* web site:

<a href="https://www.twilio.com">https://www.twilio.com</a>

You cannot send text messages without obtaining a Twilio\* API key first. You can still run the example, but without the text messages.

Pass your Twilio\* API key and authentication token to the sample program by modifying the `TWILIO_ACCT_SID` and `TWILIO_AUTH_TOKEN` keys in the `config.json` file as follows:

```
{
  "TWILIO_ACCT_SID": "YOURAPIKEY",
  "TWILIO_AUTH_TOKEN": "YOURTOKEN"
}
```

## Configuring the example

To configure the example for the Grove\* kit, just leave the `kit` key in the `config.json` set to `grove`. To configure the example for the DFRobot\* kit, change the `kit` key in the `config.json` to `dfrobot` as follows:

```
{
  "kit": "dfrobot"
}
```

To configure the example for the Arduino 101, add a `platform` key with the value `firmata` to the `config.json`, as follows:

```
{
  "kit": "grove",
  "platform": "firmata"
}
```

To configure the example for sending optional text messages, obtain an API key from the Twilio\* website as explained above, and then change the `TWILIO_ACCT_SID` and `TWILIO_AUTH_TOKEN` keys in the `config.json` file as follows:

```
{
  "kit": "grove",
  "TWILIO_ACCT_SID": "YOURAPIKEY",
  "TWILIO_AUTH_TOKEN": "YOURTOKEN"
}
```

To configure the example for the optional Microsoft Azure\*, IBM Bluemix\*, or AWS data store, change the `SERVER` and `AUTH_TOKEN` keys in the `config.json` file as follows:

```
{
  "kit": "grove",
  "SERVER": "http://intel-examples.azurewebsites.net/logger/watering-system",
  "AUTH_TOKEN": "s3cr3t"
}
```

To configure the example for both the text messages and the Microsoft Azure\*, IBM Bluemix\*, or AWS data store, change the `TWILIO_ACCT_SID`, `TWILIO_AUTH_TOKEN`, `SERVER`, and `AUTH_TOKEN` keys in the `config.json` file as follows:

```
{
  "kit": "grove",
  "TWILIO_ACCT_SID": "YOURAPIKEY",
  "TWILIO_AUTH_TOKEN": "YOURTOKEN",
  "SERVER": "http://intel-examples.azurewebsites.net/logger/watering-system",
  "AUTH_TOKEN": "s3cr3t"
}
```

For information on how to configure the example for an optional Microsoft Azure\*, IBM Bluemix\*, AT&T M2X\*, AWS\*, Predix\*, or SAP\* IoT cloud server, go to:

[https://github.com/intel-iot-devkit/iot-samples-cloud-setup](https://github.com/intel-iot-devkit/iot-samples-cloud-setup)

## Running the program 

To run this example on the board, simply enter

    $ npm install
    $ npm run
    $ ./watering-system
    
You will see output similar to below when the program is running.

![](./../../images/js/watering-system-output.png)

Refer to ![How it Works](./../README.md#how-it-works) for details on the functionality.

### IoT cloud setup (optional)

You can optionally store the data generated by this sample program using cloud-based IoT platforms from Microsoft Azure\*, IBM Bluemix\*, AT&T M2X\*, AWS\*, Predix\*, or SAP\*.

For information on how to connect to your own cloud server, go to:

[https://github.com/intel-iot-devkit/iot-samples-cloud-setup](https://github.com/intel-iot-devkit/iot-samples-cloud-setup)

### Data store server setup (optional)

Optionally, you can store the data generated by this sample program in a back-end database deployed using Microsoft Azure\*, IBM Bluemix\*, or AWS, along with Node.js\*, and a Redis\* data store.

For information on how to set up your own cloud data server, go to:

[https://github.com/intel-iot-devkit/intel-iot-examples-datastore](https://github.com/intel-iot-devkit/intel-iot-examples-datastore)

IMPORTANT NOTICE: This software is sample software. It is not designed or intended for use in any medical, life-saving or life-sustaining systems, transportation systems, nuclear systems, or for any other mission-critical application in which the failure of the system could lead to critical injury or death. The software may not be fully tested and may contain bugs or errors; it may not be intended or suitable for commercial release. No regulatory approvals for the software have been obtained, and therefore software may not be certified for use in certain countries or environments.
