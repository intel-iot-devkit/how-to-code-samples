# Smart Doorbell

Someone's knocking at the door, someone's ringing the bell. So what happens when you are not home? You can use an Intel Edison to keep track of all your visitors. When someone rings the bell, not only will it make a sound, but it can also log it to a cloud-based storage system.

## Introduction

This smart doorbell application is part of a series of how-to Intel® IoT code sample exercises using the Intel® IoT Developer Kit, Intel® Edison development platform, cloud platforms, APIs, and other technologies.

From this exercise, developers will learn how to:
- Connect the Intel® Edison development platform, a computing platform designed for prototyping and producing IoT and wearable computing products.
- Interface with the Intel® Edison platform IO and sensor repository using MRAA and UPM from the Intel® IoT Developer Kit, a complete hardware and software solution to help developers explore the IoT and implement innovative projects.
- Run this code sample in Eclipse* IoT Edition, an IDE for creating new applications that interact with sensors, actuators, and so on, enabling you to get a quick start on developing software for your Intel® Edison or Galileo board.
- Store the doorbell ring data using Azure Redis Cache* from Microsoft* Azure*, cloud services for connecting IoT solutions including data analysis, machine learning, and a variety of productivity tools to simplify the process of connecting your sensors to the cloud and getting your IoT project up and running quickly.

## What It Is

Using an Intel® Edison board, this project lets you create a smart doorbell that:
- audible notification whenever the doorbell is rung;
- visual notification whenever the doorbell is rung;
- keeps track of visitors using cloud-based data storage.


## How It Works

This "smart" doorbell will do the following when the Grove Touch Sensor is pressed:

- Make a noise with the buzzer
- Display a message on the LCD Display
- Optionally, store the total number of doorbell rings using the "Intel IoT Example Datastore" running on your own server, such as a Microsoft Azure or IBM Bluemix account.

## Hardware requirements

Grove* Starter Kit containing:

1. Intel® Edison with an Arduino* breakout board
2. [Grove* Touch Sensor](http://iotdk.intel.com/docs/master/upm/node/classes/ttp223.html)
3. [Grove* Buzzer](http://iotdk.intel.com/docs/master/upm/node/classes/buzzer.html)
4. [Grove* RGB LCD](http://iotdk.intel.com/docs/master/upm/node/classes/jhd1313m1.html)

## Software requirements

1. Eclipse IoT Edition
2. Microsoft* Azure* account

### How To Setup

To begin, clone the Intel® IoT Examples repository with Git on your computer as follows:

    $ git clone https://github.com/hybridgroup/intel-iot-examples.git

Just want to download a ZIP file? Just point your web browser to the Github repo at [https://github.com/hybridgroup/intel-iot-examples](https://github.com/hybridgroup/intel-iot-examples) and click on the "Download ZIP" button at the lower right. Once the ZIP file has finished downloading, uncompress it, and then use the files in the directory for this example.


### How To Setup

To begin, clone the Intel IoT Examples with git on your computer:

    $ git clone https://github.com/hybridgroup/intel-iot-examples.git

Not sure how to do that? [Here is an excellent guide from Github on how to get started](https://help.github.com/desktop/guides/getting-started/).

### Adding The Code To Eclipse IoT

You use the Eclipse "Import Wizard" to import an existing project into the workspace as follows:

- From the main menu bar, select "File > Import..."
![](./../../../images/Eclipse-Menu.png)

- The "Import wizard" dialog will open.
![](./../../../images/Eclipse-Menu-Select-PiW.png)

- Select "General > Existing Project into Workspace" and click on the "Next" button.
![](./../../../images/Eclipse-Menu-Select-PiW.png)

- Choose "Select root directory", then click on the associated "Browse" button to locate the directory that contains the project files.
![](./../../../images/Eclipse-Menu-Select-RootDir.png)

- Under "Projects" select the directory with the project files which you would like to import.
![](./../../../images/Eclipse-Menu-Select-PiW-RootDir.png)
- Click on the "Finish" button to import the files into Eclipse.

![](./../../../images/Eclipse-Menu-Src-Loc.png)
- Your main .cpp program will now be in your workspace under the src folder.

### Connecting The Grove Sensors

![](./../../../images/doorbell.jpg)

You will need to have the Grove Shield connected to the Arduino-compatible breakout board, in order to plug in all the various Grove devices into the Grove shield. Make sure you have the tiny VCC switch on the Grove Shield set to the "5V" position.

Plug one end of a Grove cable into the "Touch Sensor", then connect the other end to the "D4" port on the Grove Shield.

Connect one end of a Grove cable into the "Buzzer", then plug the other end into the "D5" port on the Grove Shield.

Plug one end of a Grove cable into the "RGB LCD", then connect the other end into any of the "I2C" ports on the Grove Shield.

### Intel Edison Setup

This example uses the `restclient-cpp` library to perform REST calls to the server. The code for `restclient-cpp` can be found in the `lib` directory. The `restclient-cpp` library requires the `libcurl` package, which is already installed on the Intel Edison by default.

## Data Store Server Setup

You can optionally store the data generated by this example program in a backend database deployed using Node.js, and the Redis datastore. You use your own account on a hosted service such as Microsoft Azure or IBM Bluemix.

For information on how to setup your own cloud data server, go to:

https://github.com/hybridgroup/intel-iot-examples-datastore

### Running The Example With The Cloud Server

To run the example with the optional backend datastore you need to set the `SERVER` and `AUTH_TOKEN` environment variables. You can do this in Eclipse by:

1. Select the "Run" menu and choose "Run Configurations". The "Run Configurations" dialog will be displayed.
2. Click on "doorbell" under "C/C++ Remote Application". This will display the information for your application.
3. Add the environment variables to the field for "Commands to execute before application" so it ends up looking like this, except using the server and auth token that correspond to your own setup:

```
chmod 755 /tmp/doorbell; export SERVER="http://intel-examples.azurewebsites.net/counter/doorbell/inc"; export AUTH_TOKEN="YOURTOKEN"
```

4. Click on the "Apply" button to save your new environment variables.

Now when you run your program using the "Run" button, it should be able to call your server to save the data right from the Edison.

### Running The Code On Edison

![](./../../../images/Run-Eclipse.png)

When you're ready to run the example, you can click on the "Run" icon located in the menubar at the top of the Eclipse editor.
This will compile the program using the Cross G++ Compiler, link it using the Cross G++ Linker, transfer the binary to the Edison, and then execute it on the Edison itself.

![](./../../../images/Run-Eclipse-Successful-Build.png)

After running the program you should have a similar output as in the image above.

When pressing the touch sensor you will hear the buzzer go off if your server is set up correctly you will be notified.