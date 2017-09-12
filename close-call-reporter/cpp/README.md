# Close call reporter in C++

## What it is

Using a compatible Intel® IoT Platform, this project lets you create a close-call fleet driving reporter that:

- monitors the Grove\* IR Distance Interrupter;
- monitors the Grove\* GPS;
- keeps track of close calls and logs them using cloud-based data storage.

## First time setup
For all the samples in this repository, see the ![General Setup Instructions](./../../README.md#setup) for required boards and libraries.  You need either Grove or DFRobot sensors but not both.

## Hardware requirements

### Grove\* 

Sensor | Pin
--- | ---
Grove\* IR Distance Interrupter | D2
Grove\* GPS | UART port

### DFRobot\* 

Sensor | Pin
--- | ---
IR Distance Sensor | D4
GPS cable TX (white) | RX pin
GPS cable RX (black) | TX pin
GPS cable power (red) | 5V pins
GPS cable ground (thicker black) | GND

For more specific information on the hardware requirements see ![Hardware Details](./../README.md#hardware-requirements)

## Software requirements

1. ![MRAA](https://github.com/intel-iot-devkit/mraa) and ![UPM](https://github.com/intel-iot-devkit/upm)
2. Microsoft Azure\*, IBM Bluemix\*, AT&T M2X\*, AWS\*, Predix\*, or SAP\* account (optional)
Note: The following libraries are included with the repo and already linked to in the code -jsoncpp -restclient

## Configuring the example for your hardware kit

To configure the example for the specific hardware kit that you are using, either Grove\* or DFRobot\*:

1. From the main menu, select **Project > Properties** dialog box is displayed.
![](./../../images/cpp/click-project-properties.png)
2. Expand the section **C/C++ General**. <br>Click on the **Paths and Symbols** sub-section, and click on the **Symbols** tab.
![](./../../images/cpp/click-gen-path-symbols.png)
3. Now click on **GNU C++**, and click on the **Add** button.
![](./../../images/cpp/click-gnupp-add.png)
4. In the **Name** field, enter "INTEL_IOT_KIT". In the **Value** field, enter either "GROVEKIT" (this is the default) or "DFROBOTKIT", depending on which hardware kit you wish to use.
![](./../../images/cpp/add-name-and-var.png)
5. Your new name symbol and value will now be displayed. Click **OK**.
![](./../../images/cpp/name-var-ok.png)
6. Another dialog box will appear asking to rebuild project. Click **OK**.
![](./../../images/cpp/path-symbol-rebuild-ok.png)

### Running the program

To run this example on the board, simply enter

    $ make
    $ build
    $ ./close-call-reporter

After running the program, you should see output similar to the one in the image below.<br>
![](./../../images/cpp/cpp-run-eclipse-successful-build.png)

Refer to ![How it Works](./../README.md#how-it-works) for details on the functionality.

IMPORTANT NOTICE: This software is sample software. It is not designed or intended for use in any medical, life-saving or life-sustaining systems, transportation systems, nuclear systems, or for any other mission-critical application in which the failure of the system could lead to critical injury or death. The software may not be fully tested and may contain bugs or errors; it may not be intended or suitable for commercial release. No regulatory approvals for the software have been obtained, and therefore software may not be certified for use in certain countries or environments.
