# Doorbell

## Introduction

This smart doorbell application is part of a series of how-to Intel® IoT code sample exercises using the Intel® IoT Developer Kit, Intel® Edison development platform, cloud platforms, APIs, and other technologies.

From this exercise, developers will learn how to:<br>
- Connect the Intel® Edison development platform, a computing platform designed for prototyping and producing IoT and wearable computing products.<br>
- Interface with the Intel® Edison platform IO and sensor repository using MRAA and UPM from the Intel® IoT Developer Kit, a complete hardware and software solution to help developers explore the IoT and implement innovative projects.<br>
- Run this code sample in Eclipse* IoT Edition, an IDE for creating new applications that interact with sensors, actuators, and so on, enabling you to get a quick start on developing software for your Intel® Edison or Galileo board.<br>
- Store the doorbell ring data using Azure Redis Cache* from Microsoft* Azure* or Redis Store* from IBM* Bluemix*, both cloud services for connecting IoT solutions including data analysis, machine learning, and a variety of productivity tools to simplify the process of connecting your sensors to the cloud and getting your IoT project up and running quickly.

## What it is

Using an Intel® Edison board, this project lets you create a smart doorbell that:<br>
- audible notification whenever the doorbell is rung;<br>
- visual notification whenever the doorbell is rung;<br>
- keeps track of visitors using cloud-based data storage.

## How tt works

Naturally, this "smart" doorbell make a noise with the buzzer when the I2C Touch Sensor is pressed. In addition, it displays a message on the LCD Display.

Optionally, doorbell ring data can also be stored using the Intel® IoT Example Datastore running in your own Microsoft* Azure* or IBM* Bluemix* account.

## Hardware requirements

Grove* Starter Kit containing:

1. Intel® Edison with an Arduino* breakout board
2. [Grove* Touch Sensor](http://iotdk.intel.com/docs/master/upm/node/classes/ttp223.html)
3. [Grove* Buzzer](http://iotdk.intel.com/docs/master/upm/node/classes/buzzer.html)
4. [Grove* RGB LCD](http://iotdk.intel.com/docs/master/upm/node/classes/jhd1313m1.html)

## Software requirements

1. [Eclipse* Iot version](https://software.intel.com/en-us/eclipse-getting-started-guide)
2. Microsoft* Azure* account

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

### Connecting the Grove* sensors

![](./../../../images/js/doorbell.jpg)

You will need to have the Grove* Shield connected to the Arduino*-compatible breakout board, in order to plug in all the various Grove* devices into the Grove* shield. Make sure you have the tiny VCC switch on the Grove* Shield set to **5V**.

1. Plug one end of a Grove* cable into the Grove* Touch Sensor, then connect the other end to the "D4" port on the Grove* Shield.
2. Plug one end of a Grove* cable into the Grove* Buzzer, then connect the other end to the "D5" port on the Grove* Shield.
3. Plug one end of a Grove* cable into the Grove* RGB LCD, then connect the other end to any of the "I2C" ports on the Grove* Shield.

### Intel® Edison setup

This example also uses the **restclient-cpp** library to perform REST calls to the remote data server. The code can be found in the **lib** directory. The **restclient-cpp** library requires the **libcurl** package, which is already installed on Intel® Edison by default.

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
3. In the **Commands to execute before application** field, add the environment variables so it looks like this, except using the server and authentication token that correspond to your own setup:<br>
```
chmod 755 /tmp/doorbell;export API_KEY="YOURKEY"; export SERVER="http://intel-examples.azurewebsites.net/counter/logger/alarm-clock"; export AUTH_TOKEN="YOURTOKEN"
```

4. Click **Apply** to save your new environment variables.

Now when you run your program using the **Run** button, it should be able to call your server to save the data right from Intel® Edison.

### Running the code on Intel® Edison

![](./../../../images/cpp/cpp-run-eclipse.png)

When you're ready to run the example, click **Run** at the top menu bar in Eclipse*. This compiles the program using the Cross G++ Compiler, links it using the Cross G++ Linker, transfers the binary to Intel® Edison, and then executes it on the board itself.

![](./../../../images/cpp/cpp-run-eclipse-successful-build.png)

After running the program, you should see output similar to the one in the image above.

![](./../../../images/cpp/doorbell-lcd.jpg)

When the program uploads and runs on the Intel® Edison board, the Grove* RGB LCD display will show the above message. When pressing the Grove* Touch sensor you will hear the buzzer go off, and if your server is set up correctly, you will be notified.
