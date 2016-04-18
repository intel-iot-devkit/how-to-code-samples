# Plant lighting system in C++

## Introduction

This automatic plant lighting system monitor application is part of a series of how-to Intel IoT code sample exercises using the Intel® IoT Developer Kit, Intel® Edison board, cloud platforms, APIs, and other technologies.

From this exercise, developers will learn how to:

- Connect the Intel® Edison board, a computing platform designed for prototyping and producing IoT and wearable computing products.
- Interface with the Intel® Edison board IO and sensor repository using MRAA and UPM from the Intel® IoT Developer Kit, a complete hardware and software solution to help developers explore the IoT and implement innovative projects.
- Run these code samples in the Intel® System Studio IoT Edition (Eclipse\* IDE for C/C++ and Java\* development) for creating applications that interact with sensors and actuators, enabling a quick start for developing software for the Intel® Edison or Intel® Galileo board.
- Set up a web application server to set the lighting time and store this data using Azure Redis Cache\* from Microsoft\* Azure\*, Redis Store\* from IBM\* Bluemix\*, or ElastiCache\* using Redis\* from Amazon\* Web Services\* (AWS\*), different cloud services for connecting IoT solutions including data analysis, machine learning, and a variety of productivity tools to simplify the process of connecting your sensors to the cloud and getting your IoT project up and running quickly.
- Invoke the services of the Twilio\* API for sending text messages.
- Set up a MQTT-based server using IoT Hub\* from Microsoft\* Azure\*, IoT\* from IBM\* Bluemix\*, or IoT\* from Amazon\* Web Services\* (AWS\*), different cloud machine to machine messaging services based on the industry standard MQTT protocol.

## What it is

Using an Intel® Edison board, this project lets you create an automatic plant lighting monitor system that:

- checks if a separate automated lighting system is turned on or off based on a configurable schedule, by using a light sensor;
- can be accessed with your mobile phone via a built-in web interface to set the lighting periods;
- additionally monitors water levels using a connected moisture sensor;
- logs events from the lighting system, using cloud-based data storage;
- sends text messages to alert the user if the system is not working as expected.

## How it works

This lighting system has a few useful features.

You can set the lighting system schedule via a web page served directly from the Intel® Edison board, using your mobile phone.

If the lighting is supposed to be on, but the light sensor does not detect light, the application sends an SMS alert to a specified number through Twilio\*.

It also automatically checks the moisture sensor data at specified intervals and logs that information.

Optionally, all data can be stored using the Intel IoT Examples Datastore or an MQTT server running in your own Microsoft\* Azure\*, IBM\* Bluemix\*, or AWS\* account.

## Hardware requirements

Grove* Indoor Environment Kit containing:

1. Intel® Edison board with an Arduino* breakout board
2. Grove* Base Shield V2
3. [Grove* Moisture Sensor](http://iotdk.intel.com/docs/master/upm/node/classes/grovemoisture.html)
4. [Grove* Light Sensor](http://iotdk.intel.com/docs/master/upm/node/classes/grovelight.html)
5. [Grove* RGB LCD](http://iotdk.intel.com/docs/master/upm/node/classes/jhd1313m1.html)

## Software requirements

1. [Intel® System Studio IoT Edition (Eclipse* IDE for C/C++ and Java* development)](https://software.intel.com/en-us/eclipse-getting-started-guide)
2. Microsoft\* Azure\*, IBM\* Bluemix\*, or AWS\* account
3. Twilio\* account

## How to set up

To begin, clone the **How-To Intel IoT Code Samples** repository with Git* on your computer as follows:

    $ git clone https://github.com/intel-iot-devkit/how-to-code-samples.git

Want to download a .zip file? In your web browser, go to [https://github.com/intel-iot-devkit/how-to-code-samples](https://github.com/intel-iot-devkit/how-to-code-samples) and click the **Download ZIP** button at the lower right. Once the .zip file is downloaded, uncompress it, and then use the files in the directory for this example.

### Adding the program to Eclipse*

In Eclipse\*, select **Import Wizard** to import an existing project into the workspace as follows:

1. From the main menu, select **File > Import**.<br>
![](./../../images/cpp/cpp-eclipse-menu.png)
2. The **Import Wizard** dialog box opens. Select **General > Existing Project into Workspace** and click **Next**.<br>
![](./../../images/cpp/cpp-eclipse-menu-select-epiw.png)
3. Click **Select root directory** and then the associated **Browse** button to locate the directory that contains the project files.<br>
![](./../../images/cpp/cpp-eclipse-menu-select-rootdir.png)
4. Under **Projects**, select the directory with the project files you'd like to import and click **OK** and then **Finish** to import the files into Eclipse*.<br>
![](./../../images/cpp/cpp-eclipse-menue-epiw-rootdir.png)
5. Your main .cpp program is now displayed in your workspace under the **src** folder.<br>
![](./../../images/cpp/cpp-eclipse-menu-src-loc.png)

### Connecting the Grove* sensors

You need to have a Grove* Base Shield V2 connected to an Arduino\*-compatible breakout board to plug all the Grove* devices into the Grove* Base Shield V2. Make sure you have the tiny VCC switch on the Grove* Base Shield V2 set to **5V**.

1. Plug one end of a Grove* cable into the Grove* Light Sensor, and connect the other end to the A0 port on the Grove* Base Shield V2.<br>
![](./../../images/js/lighting.jpg)
2. Plug one end of a Grove* cable into the Grove* Moisture Sensor, and connect the other end to the A1 port on the Grove* Base Shield V2.
3. Plug one end of a Grove* cable into the Grove* RGB LCD, and connect the other end to any of the I2C ports on the Grove* Base Shield V2.

### Intel® Edison board setup

This example uses the **restclient-cpp** library to perform REST calls to the remote data server. The code can be found in the **lib** directory. The **restclient-cpp** library requires the **libcurl** package, which is already installed on the Intel® Edison board by default.

In addition, this example uses the **twilio-cplusplus** library to perform REST calls to the Twilio* SMS server. The code for **twilio-cplusplus** can be found in the **lib** directory. The **twilio-cplusplus** library requires the **ssl** and **crypto** packages, which are already installed on the Intel® Edison board by default.

This example also uses the Crow* web micro-framework to provide a simple-to-use, yet powerful web server. The Crow* library requires the **libboost** package be installed on the Intel® Edison board, as well as adding the needed include and lib files to the Eclipse* Cross G++ Compiler and Cross G++ Linker.

1. Update opkg base feeds so you can install the needed dependencies. Establish an SSH connection to the Intel® Edison board and run the following command:<br>

        vi /etc/opkg/base-feeds.conf

2. Edit the file so that it contains the following:<br>

        src/gz all http://repo.opkg.net/edison/repo/all
        src/gz edison http://repo.opkg.net/edison/repo/edison
        src/gz core2-32 http://repo.opkg.net/edison/repo/core2-32

3. Save the file by pressing **Esc**, then **:**, then **q**, and **Enter**.

This only needs to be done once per Intel® Edison board, so if you've already done it, you can skip to the next step.

Install the **boost** libraries onto the Intel® Edison board by running the following command:

    opkg update
    opkg install boost-dev

### Copy the libraries

You need to copy the libraries and include files from the board to your computer where you're running Eclipse* so the Cross G++ Compiler and Cross G++ Linker can find them. The easiest way to do this is by running the `scp` command from your computer (NOT the Intel® Edison board), as follows:

    scp -r USERNAME@xxx.xxx.x.xxx:/usr/include/boost ~/Downloads/iotdk-ide-linux/devkit-x86/sysroots/i586-poky-linux/usr/include
    scp USERNAME@xxx.xxx.x.xxx:/usr/lib/libboost* ~/Downloads/iotdk-ide-linux/devkit-x86/sysroots/i586-poky-linux/usr/lib

Change `USERNAME@xxx.xxx.x.xxx` to match whatever username and IP address you set your board to.

Change `~/Downloads/iotdk-ide-linux` to match the location on your computer where you installed the Intel® IoT Developer Kit.

### Copy the libraries on Windows*

We have a helpful link to get this set up here:

[using-winscp.md](./../../docs/cpp/using-winscp.md)

Note: you need to turn SSH on by running the `configure_edison --password` command on the board. Once you set the password, make sure you write it down. You only need to do this once and it is set when you reboot the Intel® Edison board.

### Twilio* API Key

To optionally send text messages, you need to register for an account and get an API key from the Twilio* web site:

[https://www.twilio.com](https://www.twilio.com)

You cannot send text messages without obtaining a Twilio* API key first. You can still run the example, but without SMS alerts.

### Datastore server setup

Optionally, you can store the data generated by this sample program in a backend database deployed using Microsoft* Azure\*, IBM* Bluemix\*, or AWS\*, along with Node.js\*, and a Redis\* data store.

For information on how to set up your own cloud data server, go to:

[https://github.com/intel-iot-devkit/intel-iot-examples-datastore](https://github.com/intel-iot-devkit/intel-iot-examples-datastore)

### MQTT server setup

You can also optionally store the data generated by this sample program using [MQTT](http://mqtt.org/), a Machine To Machine messaging server. You can use MQTT to connect to Microsoft\* Azure\*, IBM\* Bluemix\*, or AWS\*.

For information on how to connect to your own cloud MQTT messaging server, go to:

[https://github.com/intel-iot-devkit/intel-iot-examples-mqtt](https://github.com/intel-iot-devkit/intel-iot-examples-mqtt)

### Connecting your Intel® Edison board to Eclipse*

1. In the bottom left corner, right-click anywhere on the **Target SSH Connections** tab and select **New > Connection**.<br>
![](./../../images/cpp/cpp-connection-eclipse-ide-win.png)
2. The **Intel(R) IoT Target Connection** window appears. In the **Filter** field, type the name of your board.<br>
![](./../../images/cpp/cpp-connection-eclipse-ide-win2.png)
3. In the **Select one of the found connections** list, select your device name and click **OK**.<br>
![](./../../images/cpp/cpp-connection-eclipse-ide-win3.png)
4. On the **Target SSH Connections** tab, right-click your device and select **Connect**.<br>
![](./../../images/cpp/cpp-connection-eclipse-ide-win4.png)

If prompted for the username and password, the username is **root** and the password is whatever you specified when configuring the Intel® Edison board.

### Running the example with the cloud server

To run the example with the optional backend data store, you need to set the `SERVER` and `AUTH_TOKEN` environment variables. You can do this in Eclipse* as follows:

1. From the **Run** menu, select **Run Configurations**.<br> The **Run Configurations** dialog box is displayed.
2. Under **C/C++ Remote Application**, click **doorbell**.<br> This displays the information for the application.
3. In the **Commands to execute before application** field, add the environment variables so it looks like this, except using the server and authentication token that correspond to your own setup:<br>

        chmod 755 /tmp/watering-system; export SERVER="http://intel-iot-example-data.azurewebsites.net/logger/watering-system"; export AUTH_TOKEN="Enter Auth Token Here"; export TWILIO_SID="Enter Twilio SID Here"; export TWILIO_TOKEN="Enter Twilio Token Here"; export TWILIO_TO="Enter Number to Send to here Formattted 555-555-5555"; export TWILIO_FROM="Enter Number to be Sent From Here Formated 555-555-5555"

4. Click **Apply** to save your new environment variables.

Now, when you run your program using the **Run** button, it should be able to call your server to save the data right from the Intel® Edison board.

### Running the code on the Intel® Edison board

When you're ready to run the example, click **Run** at the top menu bar in Eclipse*.<br>
![](./../../images/cpp/cpp-run-eclipse.png)

This compiles the program using the Cross G++ Compiler, links it using the Cross G++ Linker, transfers the binary to the Intel® Edison board, and then executes it on the board itself.

After running the program, you should see output similar to the one in the image below.<br>
![](./../../images/cpp/cpp-run-eclipse-successful-build.png)

Successful output should be similar to the one in the image below.<br>
![](./../../images/cpp/cpp-equp-act-mon-successful-output.png)

## Regenerating HTML and CSS

If you make any changes to either the **index.html** or **styles.css** file, you need to regenerate the .hex file used to serve up the assets via the built-in Crow* web server.
We have a useful tutorial on how to use the shell script here:

[how-to-run-the-shellscript.md](./../../docs/cpp/how-to-run-the-shellscript.md)

### Setting the lighting schedule

The schedule for the lighting system is set using a single-page web interface served up from the Intel® Edison board while the sample program is running.<br>
![](./../../images/cpp/plant-ligt-app.png)

The latest data values from the connected Grove* Moisture Sensor are displayed at the bottom of the web page.

The web server runs on port `3000`, so if the Intel® Edison board is connected to Wi-Fi* on `192.168.1.13`, the address to browse to if you are on the same network is `http://192.168.1.13:3000`.
