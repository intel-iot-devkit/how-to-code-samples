# Field data reporter in JavaScript*

## Introduction

This wrist field data reporter application is part of a series of how-to Intel IoT code sample exercises using the IntelÂ® IoT Developer Kit, IntelÂ® Edison development platform, cloud platforms, APIs, and other technologies.

From this exercise, developers will learn how to:<br>
- Connect the IntelÂ® Edison development platform, a computing platform designed for prototyping and producing IoT and wearable computing products.<br>
- Interface with the IntelÂ® Edison platform IO and sensor repository using MRAA and UPM from the IntelÂ® IoT Developer Kit, a complete hardware and software solution to help developers explore the IoT and implement innovative projects.<br>
- Run this code sample in IntelÂ® System Studio IoT Edition . IntelÂ® System Studio IoT Edition lets you create and test applications on IntelÂ®-based IoT platforms.<br>
- Set up a web application server to view data using a web page served directly from IntelÂ® Edison.

## What it is

Using an IntelÂ® Edison board, this project lets you create a wrist field data reporter that:<br>
- continuously monitors the data read from the digital barometer;<br>
- displays the latest reading using the OLED display when the touch sensor is tapped;<br>
- serves the recorded data in the JSON format directly from IntelÂ® Edison.

## How it works

This wrist field data reporter can be used to sample barometer data at regular intervals.

It also provides a way to look at the latest data captured, using the QTouch* sensor and the OLED display.

These readings can be viewed or downloaded as JSON data served directly from an embedded web server running on IntelÂ® Edison.

## Hardware requirements

Xadow* Starter Kit containing:

1. IntelÂ® Edison with a Xadow* expansion board
2. [Xadow* - OLED Display](http://iotdk.intel.com/docs/master/upm/node/classes/ssd1308.html)
3. [Xadow* - QTouch Sensor](http://iotdk.intel.com/docs/master/upm/node/classes/at42qt1070.html)
4. [Xadow* - Atmospheric Pressure Sensor](http://iotdk.intel.com/docs/master/upm/node/classes/bmpx8x.html)


## Software requirements

1. IntelÂ® System Studio IoT Edition

### How to set up

To begin, clone the **How-To Intel IoT Code Samples** repository with Git* on your computer as follows:

    $ git clone https://github.com/intel-iot-devkit/how-to-code-samples.git

Want to download a .zip file? In your web browser, go to <a href="https://github.com/intel-iot-devkit/how-to-code-samples">https://github.com/intel-iot-devkit/how-to-code-samples</a> and click the **Download ZIP** button at the lower right. Once the .zip file is downloaded, uncompress it, and then use the files in the directory for this example.

## Adding the program to IntelÂ® System Studio IoT Edition

** The following screenshots are from the Alarm clock sample, however the technique for adding the program is the same, just with different source files and jars.

Open IntelÂ® System Studio IoT Edition, it will start by asking for a workspace directory. Choose one and click OK.

In IntelÂ® System Studio IoT Edition , select File -> new -> **Intel(R) IoT Java Project**:

![](./../../images/java/new project.png)

Give the project the name "FieldDataReporter" and click Next.

![](./../../images/java/project name.png)

You now need to connect to your IntelÂ® Edison board from your computer to send code to it.
Choose a name for the connection and enter the Edison's IP address in the "Target Name" field. You can also try to Search for it using the "Search Target" button. Click finish when you are done.

![](./../../images/java/Target connection.png)

You have successfully created an empty project. You now need to copy the source files and the config file to the project. 
Drag all of the files from your git repository's "src" folder into the new project's src folder in Intel® System Studio IoT Edition. Make sure previously auto-generated main class is overridden.

The project uses the following external jars: [gson-2.6.1](http://central.maven.org/maven2/com/google/code/gson/gson/2.6.1/gson-2.6.1.jar), [jetty-all-9.3.7.v20160115-uber](http://repo1.maven.org/maven2/org/eclipse/jetty/aggregate/jetty-all/9.3.7.v20160115/jetty-all-9.3.7.v20160115-uber.jar). These can be found in the Maven Central Repository. Create a "jars" folder in the project's root directory, and copy all needed jars in this folder.
In IntelÂ® System Studio IoT Edition, select all jar files in "jars" folder and  right click -> Build path -> Add to build path

![](./../../images/java/add to build path.png)

Now you need to add the UPM jar files relevant to this specific sample.
right click on the project's root -> Build path -> Configure build path. Java Build Path -> 'Libraries' tab -> click on "add external JARs..."

for this sample you will need the following jars:

1. upm_at42qt1070.jar
2. upm_bmpx8x.jar
3. upm_i2clcd.jar

The jars can be found at the IOT Devkit installation root path\iss-iot-win\devkit-x86\sysroots\i586-poky-linux\usr\lib\java

![](./../../images/java/add external jars to build path.png)

### Connecting the Xadow* sensors

![](./../../images/java/field-data.jpg)

You need to have a Xadow* expansion board connected to IntelÂ® Edison to plug in all the Xadow* devices.

1. Plug one end of a Xadow* connector into the Xadow* - OLED Display, and connect the other end to one of the side connectors on the Xadow* expansion board.

2. Plug one end of a Xadow* connector into the Xadow* - Atmospheric Pressure Sensor, and connect the other end to one of the side connectors on the Xadow* expansion board.

3. Plug one end of a Xadow* connector into the Xadow - QTouch* Sensor, and connect the other end to one of the other two connected devices.


### Datastore server setup

Optionally, you can store the data generated by this sample program in a backend database deployed using Microsoft* Azure*, IBM* Bluemix, or AWS, along with Node.js*, and a Redis* data store.

For information on how to set up your own cloud data server, go to:

<a href="https://github.com/intel-iot-devkit/intel-iot-examples-datastore">https://github.com/intel-iot-devkit/intel-iot-examples-datastore</a>


## Preparing Edison before running the project

In order for the sample to run you will need to copy some files to Edison. This can be done using SCP through SSH.

The files need to be copied from the sample repository: <br>
Jar files- external libraries in the project need to be copied to "/usr/lib/java"




## Running the program using IntelÂ® System Studio IoT Edition

When you're ready to run the example, make sure you saved all the files.

Click the **Run** icon on the toolbar of IntelÂ® System Studio IoT Edition. This runs the code on IntelÂ® Edison.

![](./../../images/java/run project.png)

### Data access via browser

![](./../../images/java/field-data-web.png)

The data collected can be downloaded as JSON directly via a web page served by the program running on Intel® Edison.

The web server runs on port 8080, so if Intel® Edison is connected to Wi-Fi* on 192.168.1.13, the address to browse to if you are on the same network is http://192.168.1.13:8080.

### Determining the IntelÂ® Edison IP address

You can determine what IP address IntelÂ® Edison is connected to by running the following command:

    ip addr show | grep wlan

You will see the output similar to the following:

    3: wlan0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast qlen 1000
        inet 192.168.1.13/24 brd 192.168.1.255 scope global wlan0

The IP address is shown next to `inet`. In the example above, the IP address is `192.168.1.13`.
