# Alarm clock in C++

## What it is

Using a compatible Intel® IoT Platform, this project lets you create a smart alarm clock that:

- can be accessed with your mobile phone via the built-in web interface to set the alarm time.
- displays live weather data on the LCD.
- keeps track of how long it takes you to wake up each morning, using cloud-based data storage.

## First time setup
For all the samples in this repository, see the ![General Setup Instructions](./../../README.md#setup) for required boards and libraries.  You need either Grove or DFRobot sensors but not both.

## Hardware requirements

### Grove\* 

Sensor | Pin
--- | ---
Grove\* Rotary Analog Sensor | A0
Grove Button | D4
Grove Buzzer | D5
Grove RGB LCD | I2C

### DFRobot\* 

Sensor | Pin
--- | ---
Buzzer | A1
Button | A2
Rotary Analog Sensor | A3

For more specific information on the hardware requirements see ![Hardware Details](./../README.md#hardware-requirements)

## Software requirements

1. ![MRAA](https://github.com/intel-iot-devkit/mraa) and ![UPM](https://github.com/intel-iot-devkit/upm) 
2. Microsoft\* Azure\*, IBM\* Bluemix\*, AT&T\* M2X\*, AWS\*, GE\* Predix\*, or SAP\* account (optional)
3. Weather Underground\* API key
Note: The following libraries are included with the repo and already linked to in the code -jsoncpp -restclient

### Additional setup

You also want to set the current time zone to match the zone you are in. Do this by using the `timedatectl` program on the board. For example:

    timedatectl set-timezone America/Los_Angeles

### Weather Underground\* API key

To optionally fetch the real-time weather information, you need to get an API key from the Weather Underground\* web site:

[http://www.wunderground.com/weather/api](http://www.wunderground.com/weather/api)

You can still run the example, but without a Weather Underground\* API key you cannot retrive weather data.

Pass your Weather Underground\* API key to the sample program by modifying the `WEATHER_API_KEY` key in the `config.json` file as follows:

1. From the **Run** menu, select **Run Configurations**.<br> The **Run Configurations** dialog box is displayed.
2. Under **C/C++ Remote Application**, click **alarm-clock**.<br> This displays the information for your application.
3. In the **Commands to execute before application** field, add the following environment variables, except use the API key that corresponds to your own setup:<br>

        chmod 755 /tmp/alarm-clock; export API_KEY="YOURKEY"

4. Click **Apply** to save your new environment variables.

Now when you run your program using the **Run** button, it should be able to retrieve real-time weather data from the Intel® IoT Platform.

### Running the program

To run this example on the board, simply enter

    $ make
    $ build
    $ ./equipment-activity

After running the program, you should see output similar to the text below.<br>
```
[Upload Complete] forecast: Clear
Alarm duration (ms): 3256
Connecting to MQTT server...
MQTT message published: { d: {value: '3256'} }
```



When the program uploads to the Intel® IoT Platform, your RGB LCD shows the current time.<br>
![](./../../images/cpp/alarm-clock-lcd.jpg)

Refer to ![How it Works](./../README.md#how-it-works) for details on the functionality.

## Regenerating HTML and CSS

If you make any changes to either the **index.html** or **styles.css** file, you need to regenerate the .hex file used to serve up the assets via the built-in Crow\* web server.
For help using the shell script, go to this link:

[how-to-run-the-shellscript.md](./../../docs/cpp/how-to-run-the-shellscript.md)

IMPORTANT NOTICE: This software is sample software. It is not designed or intended for use in any medical, life-saving or life-sustaining systems, transportation systems, nuclear systems, or for any other mission-critical application in which the failure of the system could lead to critical injury or death. The software may not be fully tested and may contain bugs or errors; it may not be intended or suitable for commercial release. No regulatory approvals for the software have been obtained, and therefore software may not be certified for use in certain countries or environments.
