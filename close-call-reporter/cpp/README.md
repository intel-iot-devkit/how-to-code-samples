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

You will see output similar to below when the program is running.

```
close call at $GPGGA,022655.000,3405.0861,N,11816.6856,W,2,8,1.07,154.4,M,-33.6,M,0000,0000*65
Connecting to MQTT server...
MQTT message published: { d: { value: '2016-04-22T02:26:56.373Z $GPGGA,022655.000,3405.0861,N,11816.6856,W,2,8,1.07,154.4,M,-33.6,M,0000,0000*65\r'
} }
```

After running the program, you should see output similar to the one in the image below.<br>
![](./../../images/cpp/cpp-run-eclipse-successful-build.png)

Refer to ![How it Works](./../README.md#how-it-works) for details on the functionality.

### Running the example with the cloud server (optional)

To run the example with the optional backend data store, you need to set the `SERVER` and `AUTH_TOKEN` environment variables. You can do this in Intel® System Studio as follows:

1. From the **Run** menu, select **Run Configurations**.<br> The **Run Configurations** dialog box is displayed.
2. Under **C/C++ Remote Application**, click **close-call**.<br> This displays the information for the application.
3. In the **Commands to execute before application** field, add the following environment variables, except use the server and authentication token that correspond to your own setup:<br>

        chmod 755 /tmp/close-call;export SERVER="http://intel-examples.azurewebsites.net/logger/close-call"; export AUTH_TOKEN="YOURTOKEN"

4. Click **Apply** to save your new environment variables.

Now when you run your program using the **Run** button, it should be able to call your server to save the data right from the Intel® Edison board or Intel® IoT Gateway.

IMPORTANT NOTICE: This software is sample software. It is not designed or intended for use in any medical, life-saving or life-sustaining systems, transportation systems, nuclear systems, or for any other mission-critical application in which the failure of the system could lead to critical injury or death. The software may not be fully tested and may contain bugs or errors; it may not be intended or suitable for commercial release. No regulatory approvals for the software have been obtained, and therefore software may not be certified for use in certain countries or environments.
