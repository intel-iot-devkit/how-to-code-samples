# Alarm clock

## Introduction

This smart alarm clock application is part of a series of how-to Intel IoT code sample exercises using the Intel® IoT Developer Kit, Intel® Edison development platform, cloud platforms, APIs, and other technologies.

From this exercise, developers will learn how to:<br>
- Connect the Intel® Edison development platform, a computing platform designed for prototyping and producing IoT and wearable computing products.<br>
- Interface with the Intel® Edison platform IO and sensor repository using MRAA and UPM from the Intel® IoT Developer Kit, a complete hardware and software solution to help developers explore the IoT and implement innovative projects.<br>
- Run this code sample in Intel® System Studio IoT Edition . Intel® System Studio lets you create and test applications on Intel®-based IoT platforms.<br>
- Set up a web application server to set the alarm time and store this alarm data using Azure Redis Cache* from Microsoft* Azure\*, cloud services for connecting IoT solutions including data analysis, machine learning, and a variety of productivity tools to simplify the process of connecting your sensors to the cloud and getting your IoT project up and running quickly.<br>
- Invoke the services of the Weather Underground* API for accessing weather data.

## What it is

Using an Intel® Edison board, this project lets you create a smart alarm clock that:<br>
- can be accessed with your mobile phone via the built-in web interface to set the alarm time;<br>
- displays live weather data on the LCD;<br>
- keeps track of how long it takes you to wake up each morning, using cloud-based data storage.

## How it works

This smart alarm clock has a number of useful features. Set the alarm using a web page served directly from the Intel® Edison board, using your mobile phone. When the alarm goes off, the buzzer sounds, and the LCD indicates it’s time to get up. The rotary dial can be used to adjust the brightness of the display.

In addition, the smart alarm clock can access daily weather data via the Weather Underground* API and use it to change the color of the LCD.
Optionally, all data can also be stored using the Intel IoT Examples Datastore running in your own Microsoft* Azure* account.

## Hardware requirements

Grove* Starter Kit Plus containing:

1. Intel® Edison platform with an Arduino* breakout board
2. [Grove* Rotary Analog Sensor](http://iotdk.intel.com/docs/master/upm/java/classupm__grove_1_1_grove_rotary.html)
3. [Grove* Buzzer](http://iotdk.intel.com/docs/master/upm/java/classupm__buzzer_1_1_buzzer.html).
4. [Grove* Button](http://iotdk.intel.com/docs/master/upm/java/classupm__grove_1_1_grove_button.html)
5. [Grove* RGB LCD](http://iotdk.intel.com/docs/master/upm/java/classupm__i2clcd_1_1_jhd1313m1.html)

## Software requirements

1. Intel® System Studio IoT Edition
2. Microsoft* Azure* account
3. Weather Underground* API key

### How to set up

To begin, clone the **How-To Intel IoT Code Samples** repository with Git* on your computer as follows:

    $ git clone https://github.com/intel-iot-devkit/how-to-code-samples.git

Want to download a .zip file? In your web browser, go to <a href="https://github.com/intel-iot-devkit/how-to-code-samples">https://github.com/intel-iot-devkit/how-to-code-samples</a> and click the **Download ZIP** button at the lower right. Once the .zip file is downloaded, uncompress it, and then use the files in the directory for this example.

## Adding the program to Intel® System Studio IoT Edition

 ** The following screenshots are from the Alarm clock sample, however the technique for adding the program is the same, just with different source files and jars. 

Open Intel® System Studio IoT Edition, it will start by asking for a workspace directory. Choose one and click OK.

In Intel® System Studio IoT Edition , select File -> new -> **Intel(R) IoT Java Project**:

![](./../../images/java/new project.png)

Give the project the name "AlarmClock" and click Next.

![](./../../images/java/project name.png)

You now need to connect to your Intel® Edison board from your computer to send code to it.
Choose a name for the connection and enter IP address of the Intel® Edison board in the "Target Name" field. You can also try to Search for it using the "Search Target" button. Click finish when you are done.

![](./../../images/java/Target connection.png)

You have successfully created an empty project. You now need to copy the source files and the config file to the project. 
Drag all of the files from your git repository's "src" folder into the new project's src folder in Intel® System Studio IoT Edition. Make sure previously auto-generated main class is overridden.

The project uses the following external jars: [gson-2.6.1](http://central.maven.org/maven2/com/google/code/gson/gson/2.6.1/gson-2.6.1.jar), [jetty-all-9.3.7.v20160115-uber](http://repo1.maven.org/maven2/org/eclipse/jetty/aggregate/jetty-all/9.3.7.v20160115/jetty-all-9.3.7.v20160115-uber.jar), [joda-time-2.9.2](http://repo.maven.apache.org/maven2/joda-time/joda-time/2.9.2/joda-time-2.9.2.jar). These can be found in the Maven Central Repository. Create a "jars" folder in the project's root directory, and copy all needed jars in this folder.
In Intel® System Studio IoT Edition, select all jar files in "jars" folder and  right click -> Build path -> Add to build path

![](./../../images/java/add to build path.png)

Now you need to add the UPM jar files relevant to this specific sample.
right click on the project's root -> Build path -> Configure build path. Java Build Path -> 'Libraries' tab -> click on "add external JARs..."

for this sample you will need the following jars:

1. upm_buzzer.jar
2. upm_grove.jar
3. upm_i2clcd.jar

The jars can be found at the IOT Devkit installation root path\iss-iot-win\devkit-x86\sysroots\i586-poky-linux\usr\lib\java

![](./../../images/java/add external jars to build path.png)

### Connecting the Grove* sensors

![](./../../images/java/alarm-clock.jpg)

You need to have a Grove* Shield connected to an Arduino\*-compatible breakout board to plug all the Grove* devices into the Grove* Shield. Make sure you have the tiny VCC switch on the Grove* Shield set to **5V**.

1. Plug one end of a Grove* cable into the Grove* Rotary Analog Sensor, and connect the other end to the A0 port on the Grove* Shield.

2. Plug one end of a Grove* cable into the Grove* Button, and connect the other end to the D4 port on the Grove* Shield.

3. Plug one end of a Grove* cable into the Grove* Buzzer, and connect the other end to the D5 port on the Grove* Shield.

4. Plug one end of a Grove* cable into the Grove* RGB LCD, and connect the other end to any of the I2C ports on the Grove* Shield.

### Weather Underground* API key

To optionally fetch the real-time weather information, you need to get an API key from the Weather Underground* web site:

<a href="http://www.wunderground.com/weather/api/">http://www.wunderground.com/weather/api</a>

You cannot retrieve weather conditions without obtaining a Weather Underground* API key first. You can still run the example, but without the weather data.

Pass your Weather Underground* API key to the sample program by modifying the `WEATHER_API_KEY` key in the `config.properties` file as follows:

```

  WEATHER_API_KEY="YOURAPIKEY"

```

### Datastore server setup

Optionally, you can store the data generated by this sample program in a backend database deployed using Microsoft* Azure*, IBM* Bluemix, or AWS, along with Node.js*, and a Redis* data store.

For information on how to set up your own cloud data server, go to:

<a href="https://github.com/intel-iot-devkit/intel-iot-examples-datastore">https://github.com/intel-iot-devkit/intel-iot-examples-datastore</a>

## Configuring the example

To configure the example for the optional real-time weather data, obtain a key from the Weather Underground* web site as documented above, and then change the `WEATHER_API_KEY` and `LOCATION` keys in the `config.properties` file as follows:

```

  WEATHER_API_KEY: "YOURAPIKEY"
  LOCATION: "San_Francisco"

```

To configure the example for the optional Microsoft* Azure* data store, change the `SERVER` and `AUTH_TOKEN` keys in the `config.properties` file as follows:

```

  SERVER: "http://intel-examples.azurewebsites.net/logger/alarm-clock"
  AUTH_TOKEN: "s3cr3t"

```

To configure the example for both the weather data and the Microsoft* Azure* data store, change the `WEATHER_API_KEY`, `LOCATION`, `SERVER`, and `AUTH_TOKEN` keys in the `config.properties` file as follows:

```

  WEATHER_API_KEY: "YOURAPIKEY"
  LOCATION: "San_Francisco"
  SERVER: "http://intel-examples.azurewebsites.net/logger/alarm-clock"
  AUTH_TOKEN: "s3cr3t"

```
## Preparing the Intel® Edison board before running the project

In order for the sample to run you will need to copy some files to the Intel® Edison board. This can be done using SCP through SSH.<br>
Two sorts of files need to be copied from the sample repository:

1. Jar files- external libraries in the project need to be copied to "/usr/lib/java"
2. web files- files within site_contents folder need to be copied to "/var/AlarmClock"



## Running the program using Intel® System Studio IoT Edition

When you're ready to run the example, make sure you saved all the files.

Click the **Run** icon on the toolbar of Intel® System Studio IoT Edition. This runs the code on the Intel® Edison board.

![](./../../images/java/run project.png)



You will see output similar to the following when the program is running.

![](./../../images/java/looks when running.png)

### Setting the alarm

![](./../../images/java/alarm-clock-web.png)

The alarm is set using a single-page web interface served directly from the Intel® Edison board while the sample program is running.

The web server runs on port `8080`, so if the Intel® Edison board is connected to Wi-Fi* on `192.168.1.13`, the address to browse to if you are on the same network is `http://192.168.1.13:8080`.

### Determining the IP address of the Intel® Edison board

You can determine what IP address the Intel® Edison board is connected to by running the following command:

    ip addr show | grep wlan

You will see the output similar to the following:

    3: wlan0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast qlen 1000
        inet 192.168.1.13/24 brd 192.168.1.255 scope global wlan0

The IP address is shown next to `inet`. In the example above, the IP address is `192.168.1.13`.
