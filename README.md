# Home fall tracker

## Introduction

This home fall tracker application is part of a series of how-to Intel® IoT code sample exercises using the Intel® IoT Developer Kit, Intel® Edison development platform, cloud platforms, APIs, and other technologies.

From this exercise, developers will learn how to:
- Connect the Intel® Edison development platform, a computing platform designed for prototyping and producing IoT and wearable computing products.
- Interface with the Intel® Edison platform IO and sensor repository using MRAA and UPM from the Intel® IoT Developer Kit, a complete hardware and software solution to help developers explore the IoT and implement innovative projects.
- Run this code sample in Eclipse* IoT Edition, an IDE for creating new applications that interact with sensors, actuators, and so on, enabling you to get a quick start on developing software for your Intel® Edison or Galileo board.
- Store detected falls using Azure Redis Cache* from Microsoft* Azure* or Redis Store* from IBM* Bluemix*, both cloud services for connecting IoT solutions including data analysis, machine learning, and a variety of productivity tools to simplify the process of connecting your sensors to the cloud and getting your IoT project up and running quickly.
- Invoke the services of the Twilio* API for sending SMS messages.

## What it is

Using an Intel® Edison board, this project lets you create a home fall tracker bracelet that:
- monitors for possible falls using the accelerometer;
- when a possible fall is detected, it sends an SMS alert;
- displays information that jhelp is on the way using the OLED display;
- keeps track of detected devices, using cloud-based data storage.

## How it works

The home fall tracker bracelet will detect potential falls with the accelerometer. If it detects a possible fall, it will send an SMS alert via Twilio* and let the user know that their caretaker has been notified, and help is on the way.

Optionally, all data can be stored using the Intel® IoT Example Datastore running in your own Microsoft* Azure* or IBM* Bluemix* account.

## Hardware requirements

Xadow* Starter Kit containing:

1. Intel® Edison with Xadow*-Edison Expansion Board
2. [Xadow - OLED display](http://iotdk.intel.com/docs/master/upm/node/classes/ssd1308.html)
2. [Xadow - 3-Axis Accelerometer](http://iotdk.intel.com/docs/master/upm/node/classes/adxl345.html)

## Software requirements

1. [Eclipse*](https://software.intel.com/en-us/eclipse-getting-started-guide)
2. Microsoft* Azure* or IBM* Bluemix* account
3. Twilio* account

## How to set up

To begin, clone the **How-To Intel IoT Code Samples** repository with Git* on your computer as follows:

    $ git clone https://github.com/intel-iot-devkit/intel-iot-examples.git

Want to download a .zip file? In your web browser, go to [https://github.com/intel-iot-devkit/intel-iot-examples](https://github.com/intel-iot-devkit/intel-iot-examples) and click the **Download ZIP** button at the lower right. Once the .zip file is downloaded, uncompress it, and then use the files in the directory for this example.

### Adding the program to Eclipse*

In Eclipse*, select **Import Wizard** to import an existing project into the workspace as follows:

1. From the main menu, select **File > Import**.<br>
![](./../../../images/cpp/cpp-eclipse-menu.png)

2. The **Import Wizard** dialog box opens.
![](./../../../images/cpp/cpp-eclipse-menu-select-epiw.png)

3. Select **General > Existing Project into Workspace** and click **Next**.
![](./../../../images/cpp/cpp-eclipse-menue-epiw-rootdir.png)

4. Click **Select root directory** and then the associated **Browse** button to locate the directory that contains the project files.
![](./../../../images/cpp/cpp-eclipse-menu-select-rootdir.png)

5. Under **Projects**, select the directory with the project files you'd like to import.
![](./../../../images/cpp/cpp-eclipse-menue-epiw-rootdir.png)

6. Click **Finish** to import the files into Eclipse*.
![](./../../../images/cpp/cpp-eclipse-menu-src-loc.png)

Your main .cpp program is now displayed in your workspace under the **src** folder.

### Connecting The Xadow* Sensors

![](./../../../images/js/fall-tracker.jpg)

You need to have a Xadow* expansion board connected to Intel® Edison to plug in all the Xadow* devices.

Plug one end of a Xadow* connector into the Xadow* OLED, and connect the other end to one of the side connectors on the Xadow* expansion board.

### Intel® Edison setup

This example uses the **restclient-cpp** library to perform REST calls to the remote data server. The code can be found in the **lib** directory. The **restclient-cpp** library requires the **libcurl** package, which is already installed on Intel® Edison by default.

In addition, this example uses the **twilio-cplusplus** library to perform REST calls to the Twilio* SMS server. The code for **twilio-cplusplus** can be found in the **lib** directory. The **twilio-cplusplus** library requires the **ssl** and **crypto** packages, which are already installed on Intel® Edison by default.

### Twilio* API Key

To optionally send SMS messages, you need to register for an account and get an API key from the Twilio* web site: https://www.twilio.com/

You cannot send SMS messages without obtaining a Twilio* API key first. You can still run the example, but without the SMS alerts.

To configure the example for the optional SMS sending, obtain a key from the Twilio* web site as documented above.

### Datastore server setup

Optionally, you can store the data generated by this sample program in a backend database deployed using Microsoft* Azure\* or IBM* Bluemix*, Node.js\*, and a Redis\* data store.

For information on how to set up your own cloud data server, go to:

[https://github.com/intel-iot-devkit/intel-iot-examples-datastore](https://github.com/intel-iot-devkit/intel-iot-examples-datastore)

### Connecting your Intel® Edison to Eclipse*

![](./../../../images/cpp/cpp-connection-eclipse-ide-win.png)
1. In the bottom left corner, right-click anywhere in the **Target SSH Connections** tab and select **New > Connection**.<br> The **Intel(R) IoT Target Connection** window appears.

![](./../../../images/cpp/cpp-connection-eclipse-ide-win2.png)
2. In the **Filter** field, type the name of your board.

![](./../../../images/cpp/cpp-connection-eclipse-ide-win3.png)
3. In the **Select one of the found connections** list, select your device name and click **OK**.

![](./../../../images/cpp/cpp-connection-eclipse-ide-win4.png)
4. On the **Target SSH Connections** tab, right-click your device and select **Connect**.

If prompted for the username and password, the username is **root** and the password is whatever you specified when configuring Intel® Edison.

### Running the example with the cloud server

To run the example with the optional backend data store, you need to set the `SERVER` and `AUTH_TOKEN` environment variables. You can do this in Eclipse* as follows:

1. From the **Run** menu, select **Run Configurations**.<br> The **Run Configurations** dialog box is displayed.
2. Under **C/C++ Remote Application**, click **doorbell**.<br> This displays the information for the application.
3. In the **Commands to execute before application** field, add the environment variables so it looks like this, except using the server and authentication token that correspond to your own setup:

```
chmod 755 /tmp/fall-tracker; export SERVER="http://intel-iot-example-data.azurewebsites.net/logger/fall-tracker"; export AUTH_TOKEN="Enter Auth Token Here"; export TWILIO_SID="Enter Twilio SID Here"; export TWILIO_TOKEN="Enter Twilio Token Here"; export TWILIO_TO="Enter Number to Send to here Formattted 555-555-5555"; export TWILIO_FROM="Enter Number to be Sent From Here Formated 555-555-5555"
```

4. Click **Apply** to save your new environment variables.

Now when you run your program using the **Run** button, it should be able to call your server to save the data right from Intel® Edison.

### Running the code on Intel® Edison

![](./../../../images/cpp/cpp-run-eclipse.png)

When you're ready to run the example, click **Run** at the top menu bar in Eclipse*. This compiles the program using the Cross G++ Compiler, links it using the Cross G++ Linker, transfers the binary to Intel® Edison, and then executes it on the board itself.

![](./../../../images/cpp/cpp-run-eclipse-successful-build.png)

After running the program, you should see output similar to the one in the image above.

![](./../../../images/cpp/cpp-equp-act-mon-successful-output.png)

Successful output should be similar to the above image.

![](./../../../images/cpp/fall-detect-oled.jpg)

When the the program loads correctly your OLED screen will display "Fall Detected" in the case that a sudden motion of 5Gs or more is detected.
