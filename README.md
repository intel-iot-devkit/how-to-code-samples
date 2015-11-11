# Line Following Robot

This line following robot application is part of a series of how-to Intel® IoT code sample exercises using the Intel® IoT Developer Kit, Intel® Edison development platform, cloud platforms, APIs, and other technologies.

From this exercise, developers will learn how to:

Connect the Intel® Edison development platform, a computing platform designed for prototyping and producing IoT and wearable computing products. Interface with the Intel® Edison platform IO and sensor repository using MRAA and UPM from the Intel® IoT Developer Kit, a complete hardware and software solution to help developers explore the IoT and implement innovative projects.
Run this code sample in Eclipse* XDK IoT Edition, an IDE for creating new applications that interact with sensors, actuators, and so on, enabling you to get a quick start on developing software for your Intel® Edison or Galileo board.
Store line detection data using Azure Redis Cache* from Microsoft* Azure*.

## What It Is

Using an Intel® Edison board, this project lets you create a line following robot that:

- continuously checks the line finder sensor;
- moves forward if on the line using the stepper motors
- tries to pivot to find the line if not on it, using the stepper motors.
- logs events from the line finder, using cloud-based data storage;

## How It Works

The Line Finder robot will use the two attached motors to attempt to follow a line - keeping track of it with the Line Finder sensor. If on the line, it will move forward. Otherwise, it will pivot in place, trying to locate it using the sensor.
Optionally, it can also store system events using the "Intel IoT Example Datastore" running on your own Microsoft Azure account.

## Hardware requirements

Grove* Robotics Kit containing:

1. Intel® Edison with an Arduino* breakout board
2. Grove* Line Finder
3. Stepper Motor Controller & Stepper Motor (x2)

## Software requirements

1. [Eclipse* Iot version](https://software.intel.com/en-us/eclipse-getting-started-guide)
2. Microsoft* Azure* account

### How To Setup

To begin, clone the Intel® IoT Examples repository with Git on your computer as follows:

    $ git clone https://github.com/intel-iot-devkit/intel-iot-examples.git

Just want to download a ZIP file? Just point your web browser to the Github repo at [https://github.com/intel-iot-devkit/intel-iot-examples](https://github.com/intel-iot-devkit/intel-iot-examples) and click on the "Download ZIP" button at the lower right. Once the ZIP file has finished downloading, uncompress it, and then use the files in the directory for this example.

## Adding The Code To Eclipse IoT

You use the Eclipse "Import Wizard" to import an existing project into the workspace as follows:

- From the main menu bar, select "File > Import..."
![](./../../../images/cpp/cpp-eclipse-menu.png)

- The "Import wizard" dialog will open.
![](./../../../images/cpp/cpp-eclipse-menu-select-epiw.png)

- Select "General > Existing Project into Workspace" and click on the "Next" button.
![](./../../../images/cpp/cpp-eclipse-menue-epiw-rootdir.png)

- Choose "Select root directory", then click on the associated "Browse" button to locate the directory that contains the project files.
![](./../../../images/cpp/cpp-eclipse-menu-select-rootdir.png)

- Under "Projects" select the directory with the project files which you would like to import.
![](./../../../images/cpp/cpp-eclipse-menue-epiw-rootdir.png)
- Click on the "Finish" button to import the files into Eclipse.

![](./../../../images/cpp/cpp-eclipse-menu-src-loc.png)
- Your main .cpp program will now be in your workspace under the src folder.

### Connecting The Grove* Sensors

![](./../../../images/js/line-follower.jpg)

You need to have a Grove* Shield connected to an Arduino-compatible breakout board, to plug in all the Grove* devices into the Grove* shield. Make sure you have the tiny VCC switch on the Grove* Shield set to "5V".

You will need to power the Intel Edison with the external power adaptor that came with your starter kit, or else substitute a external 12V 1.5A power supply. You can also use an external battery, such as a 5V USB battery.

In addition, you will need a breadboard and additional 5V power supply to provide power to both motors. Note you need a separate battery or power supply for the motors. You cannot use the same power supply for both the Intel Edison and motors, so you will need either 2 batteries or 2 power supplies in total.

Each of the stepper motor controllers will need to be plugged into 4 pins on the Arduino breakout board, for it to be able to be controlled. Stepper motor controller #1 will need to be connected to pins 4, 5, 6, & 7. Stepper motor controller #2 will need to be plugged into pins 9, 10, 11, & 12. Both must be connected to ground (GND), to the 5V power coming from the Arduino breakout board (VCC), and to the separate 5V power for the motors (VM).

Plug one end of a Grove* cable into the "Grove* Line Finder", and connect the other end to the "D2" port on the Grove* Shield.

### Intel Edison Setup

This example uses the `crow` web microframework library to provide a simple to use, yet powerful web server. The `crow` library requires the `libboost` package be installed on the Intel Edison, as well as adding the needed include and lib files to the Eclipse G++ Cross Compiler and G++ Cross Linker.

Update the opkg base feeds, so you can install the needed dependencies. SSH into the Intel Edison, then run this command:

```
vi /etc/opkg/base-feeds.conf
```

Edit the file so that it contains the following:

```
src/gz all http://repo.opkg.net/edison/repo/all
src/gz edison http://repo.opkg.net/edison/repo/edison
src/gz core2-32 http://repo.opkg.net/edison/repo/core2-32
```

Now save the file, by pressing the `ESC` key, then the `:` key, then the `q` key, and hitting `ENTER`.

This only needs to be done once per Intel Edison board, so if you've already done it, you can skip to the next step.

Now, install the boost libraries onto the Intel Edison, by running:

```
opkg update
opkg install boost-dev
```

## Copy the Libraries
Next, you need to copy the libraries and include files from the Edison to your machine where you run Eclipse, so the G++ Cross Compiler and G++ Cross Linker can find them. The easiest way to do this is by running the `scp` command from your computer (NOT the Edison).

```
scp -r USERNAME@xxx.xxx.x.xxx:/usr/include/boost ~/Downloads/iotdk-ide-linux/devkit-x86/sysroots/i586-poky-linux/usr/include
scp USERNAME@xxx.xxx.x.xxx:/usr/lib/libboost* ~/Downloads/iotdk-ide-linux/devkit-x86/sysroots/i586-poky-linux/usr/lib
```

Change the `USERNAME@xxx.xxx.x.xxx` to match whatever username and IP address that you have set your Edison to.

Change `~/Downloads/iotdk-ide-linux` to match the location on your machine where you have installed the Eclipse IoT Development Kit.

## Copy the Libraries on Windows

We have a helpful link to get this set up here. https://github.com/hybridgroup/intel-iot-examples/blob/master/cpp/docs/using-winscp.md

Note that you will need to have turned on SSH by running the `configure_edison --password` command on the Edison. Once you've set the password, make sure you write it down. You only need to do this one time and it will be set when you reboot your Edison.

This example also uses the `restclient-cpp` library to perform REST calls to the remote data server. The code for `restclient-cpp` can be found in the `lib` directory. The `restclient-cpp` library requires the `libcurl` package, which is already installed on the Intel Edison by default.

You will also want to set the current time zone to match the zone you are in. You do this using the `timedatectl` program on the Edison itself. For example:

```
timedatectl set-timezone America/Los_Angeles
```

### Running The Code On Edison

When you're ready to run the example, you can click on the "Run" icon located in the menubar at the top of the Eclipse editor.
This will compile the program using the Cross G++ Compiler, link it using the Cross G++ Linker, transfer the binary to the Edison, and then execute it on the Edison itself.

## Weather Underground API Key

To optionally fetch the real-time weather information, you need to get an API key from the Weather Underground web site: http://www.wunderground.com/weather/api/

You cannot retrieve weather conditions without obtaining a Weather Underground API key first. You can still run the example, but without the weather data.

To run the example with the optional weather data intefgration, you need to set the `API_KEY` environment variable. You can do this in Eclipse by:

1. Select the "Run" menu and choose "Run Configurations". The "Run Configurations" dialog will be displayed.
2. Click on "alarm-clock" under "C/C++ Remote Application". This will display the information for your application.
3. Add the environment variables to the field for "Commands to execute before application" so it ends up looking like this, except using the API key that correspond to your own setup:

```
chmod 755 /tmp/alarm-clock; export API_KEY="YOURKEY"
```

4. Click on the "Apply" button to save your new environment variables.

Now when you run your program using the "Run" button, it should be able to retrieve real-time weather data from the Edison.

## Datastore Server Setup

Optionally, you can store the data generated by this example program in a backend database deployed using Microsoft* Azure* or IBM* Bluemix*, Node.js, and a Redis data store.

For information on how to setup your own cloud data server, go to:

https://github.com/intel-iot-devkit/intel-iot-examples-datastore

### Connecting Your Edison to the Eclipse IDE

![](./../../../images/cpp/cpp-connection-eclipse-ide-win.png)
1. In the bottom left corner right-click in the area "Target SSH Conections" select "New..." then select "Connection..." and a new screen will appear.

![](./../../../images/cpp/cpp-connection-eclipse-ide-win2.png)
2. In the "filter box" type the name of your edison. In the example mine is JustinEdison.

![](./../../../images/cpp/cpp-connection-eclipse-ide-win3.png)
3. In the "Select one of the found connections list; click on your device name. Then Ok.

![](./../../../images/cpp/cpp-connection-eclipse-ide-win4.png)
4. Your device will now appear in the "Target SSH Connections" area. Right-clickt it and select connect.
(If promted for a username and password the user is 'root' and password is whatever you set it up as when configuring the Edison board)

### Running The Example With The Cloud Server

To run the example with the optional backend datastore you need to set the `SERVER` and `AUTH_TOKEN` environment variables. You can do this in Eclipse by:

1. Select the "Run" menu and choose "Run Configurations". The "Run Configurations" dialog will be displayed.
2. Click on "doorbell" under "C/C++ Remote Application". This will display the information for the
3. Add the environment variables to the field for "Commands to execute before application" so it ends up looking like this, except using the server and auth token that correspond to your own setup:

```
chmod 755 /tmp/alarm-clock;export API_KEY="YOURKEY"; export SERVER="http://intel-examples.azurewebsites.net/counter/logger/alarm-clock"; export AUTH_TOKEN="YOURTOKEN"
```

4. Click on the "Apply" button to save your new environment variables.

Now when you run your program using the "Run" button, it should be able to call your server to save the data right from the Edison.

### Running The Code On Edison

![](./../../../images/cpp/cpp-run-eclipse.png)

When you're ready to run the example, you can click on the "Run" icon located in the menubar at the top of the Eclipse editor.
This will compile the program using the Cross G++ Compiler, link it using the Cross G++ Linker, transfer the binary to the Edison, and then execute it on the Edison itself.

![](./../../../images/cpp/cpp-run-eclipse-successful-build.png)

After running the program you should have a similar output as in the image above.
