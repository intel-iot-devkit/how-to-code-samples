# Doorbell in Java*

## Introduction

This smart doorbell application is part of a series of how-to Intel IoT code sample exercises using the Intel® IoT Developer Kit, Intel® Edison development platform, cloud platforms, APIs, and other technologies.

From this exercise, developers will learn how to:<br>
- Connect the Intel® Edison development platform, a computing platform designed for prototyping and producing IoT and wearable computing products.<br>
- Interface with the Intel® Edison platform IO and sensor repository using MRAA and UPM from the Intel® IoT Developer Kit, a complete hardware and software solution to help developers explore the IoT and implement innovative projects.<br>
- Run this code sample in Intel® IoT Developer Kit version of Eclipse, an IDE for creating applications that interact with sensors and actuators, enabling a quick start for developing software for the Intel® Edison or Intel® Galileo board.<br>
- Store the doorbell ring data using Azure Redis Cache* from Microsoft* Azure*, cloud services for connecting IoT solutions including data analysis, machine learning, and a variety of productivity tools to simplify the process of connecting your sensors to the cloud and getting your IoT project up and running quickly.

## What it is

Using an Intel® Edison board, this project lets you create a smart doorbell that:
- issues an audible notification whenever the doorbell is rung;
- issues a visual notification whenever the doorbell is rung;
- keeps track of visitors using cloud-based data storage.

## How it works

This smart doorbell makes a noise with the buzzer when the Grove* Touch Sensor is pressed. In addition, it displays a message on the LCD.

Optionally, doorbell ring data can also be stored using the Intel IoT Examples Datastore running in your own Microsoft* Azure* account.

## Hardware requirements

Grove* Starter Kit Plus containing:

1. Intel® Edison platform with an Arduino* breakout board
2. [Grove* Touch Sensor](http://iotdk.intel.com/docs/master/upm/node/classes/ttp223.html)
3. [Grove* Buzzer](http://iotdk.intel.com/docs/master/upm/node/classes/buzzer.html)
4. [Grove* RGB LCD](http://iotdk.intel.com/docs/master/upm/node/classes/jhd1313m1.html)

## Software requirements

1. Intel® IoT Developer Kit version of Eclipse
2. Microsoft* Azure* account

### How to set up

To begin, clone the **How-To Intel IoT Code Samples** repository with Git* on your computer as follows:

    $ git clone https://github.com/intel-iot-devkit/how-to-code-samples.git

Want to download a .zip file? In your web browser, go to <a href="https://github.com/intel-iot-devkit/how-to-code-samples">https://github.com/intel-iot-devkit/how-to-code-samples</a> and click the **Download ZIP** button at the lower right. Once the .zip file is downloaded, uncompress it, and then use the files in the directory for this example.

## Adding the program to the Intel® IoT Developer Kit version of Eclipse

 ** The following screenshots are from the Alarm clock sample, however the technique for adding the program is the same, just with different source files and jars.

Open Intel® System Studio IoT Edition, it will start by asking for a workspace directory. Choose one and click OK.

In Intel® System Studio IoT Edition , select File -> new -> **Intel(R) IoT Java Project**:

![](./../../images/java/new project.png)

Give the project the name "Doorbell" and click Next.

![](./../../images/java/project name.png)

You now need to connect to your Intel® Edison board from your computer to send code to it.
Choose a name for the connection and enter the IP address of the Intel® Edison board in the "Target Name" field. You can also try to Search for it using the "Search Target" button. Click finish when you are done.

![](./../../images/java/Target connection.png)

You have successfully created an empty project. You now need to copy the source files and the config file to the project.
Drag all of the files from your git repository's "src" folder into the new project's src folder in Intel® System Studio IoT Edition. Make sure previously auto-generated main class is overridden.

The project uses the following external jars: [httpcore-4.4.3](http://repo1.maven.org/maven2/org/apache/httpcomponents/httpcore/4.4.3/httpcore-4.4.3.jar), [httpclient-4.5.1](http://repo1.maven.org/maven2/org/apache/httpcomponents/httpclient/4.5.1/httpclient-4.5.1.jar). These can be found in the Maven Central Repository. Create a "jars" folder in the project's root directory, and copy all needed jars in this folder.
In Intel® System Studio IoT Edition, select all jar files in "jars" folder and  right click -> Build path -> Add to build path

![](./../../images/java/add to build path.png)

Now you need to add the UPM jar files relevant to this specific sample.
right click on the project's root -> Build path -> Configure build path. Java Build Path -> 'Libraries' tab -> click on "add external JARs..."

for this sample you will need the following jars:

1. mraa.jar
2. upm_buzzer.jar
3. upm_i2clcd.jar
4. upm_ttp223.jar

The jars can be found at the IOT Devkit installation root path\iss-iot-win\devkit-x86\sysroots\i586-poky-linux\usr\lib\java

![](./../../images/java/add external jars to build path.png)

### Connecting the Grove* sensors

![](./../../images/java/doorbell.jpg)

You need to have a Grove* Shield connected to an Arduino\*-compatible breakout board to plug all the Grove* devices into the Grove* Shield. Make sure you have the tiny VCC switch on the Grove* Shield set to **5V**.

1. Plug one end of a Grove* cable into the Grove* Touch Sensor, and connect the other end to the D4 port on the Grove* Shield.

2. Plug one end of a Grove* cable into the Grove* Buzzer, and connect the other end to the D5 port on the Grove* Shield.

3. Plug one end of a Grove* cable into the Grove* RGB LCD, and connect the other end to any of the I2C ports on the Grove* Shield.

### Microsoft* Azure* server setup

Optionally, you can store the data generated by this example program in a backend database deployed using Microsoft* Azure\*, Node.js\*, and a Redis* data store.

For information on how to set up your own cloud data server, go to:

<a href="https://github.com/intel-iot-devkit/intel-iot-examples-datastore">https://github.com/intel-iot-devkit/intel-iot-examples-datastore</a>

## Configuring the example

To configure the example for the optional Microsoft* Azure* data store, change the `SERVER` and `AUTH_TOKEN` keys in the `Doorbell.java` file as follows:

```
{
  "SERVER": "http://intel-examples.azurewebsites.net/counter/doorbell/inc",
  "AUTH_TOKEN": "s3cr3t"
}
```

## Preparing the Intel® Edison board before running the project

In order for the sample to run you will need to copy some files to the Intel® Edison board. Two sorts of files need to be copied from the sample repository:

1. Jar files- external libraries in the project need to be copied to "/usr/lib/java"


## Running the program using Intel® System Studio IoT Edition

When you're ready to run the example, make sure you saved all the files.

Click the **Run** icon on the toolbar of Intel® System Studio IoT Edition. This runs the code on the Intel® Edison board.

![](./../../images/java/run project.png)



You will see output similar to the following when the program is running.

![](./../../images/java/looks when running.png)

### Determining the IP address of the Intel® Edison board when connected via wireless

You can determine what IP address the Intel® Edison board is connected to by running the following command:

    ip addr show | grep wlan

You will see the output similar to the following:

    3: wlan0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast qlen 1000
        inet 192.168.1.13/24 brd 192.168.1.255 scope global wlan0

The IP address is shown next to `inet`. In the example above, the IP address is `192.168.1.13`.

### Determining the IP address of the Intel® Edison board when connected via ethernet

If you are using an Intel® Galileo board, which connects via an ethernet cable, run the following command:

	ip addr show

You will see the output similar to the following:

	2: enp0s20f6: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast qlen 1000
	    inet 10.237.104.143/24 brd 10.237.104.255 scope global enp0s20f6

The IP address is shown next to `inet`. In the example above, the IP address is `10.237.104.143`. The name of the interface (here: enp0s20f6) may vary, but not by much.


IMPORTANT NOTICE: This software is sample software. It is not designed or intended for use in any medical, life-saving or life-sustaining systems, transportation systems, nuclear systems, or for any other mission-critical application in which the failure of the system could lead to critical injury or death. The software may not be fully tested and may contain bugs or errors; it may not be intended or suitable for commercial release. No regulatory approvals for the software have been obtained, and therefore software may not be certified for use in certain countries or environments.
