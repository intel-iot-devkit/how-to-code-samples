# Air quality sensor in C++

## What it is
Using a compatible Intel-based platforms, this project lets you create an air quality monitor that:

- continuously monitors the air quality for airborne contaminants.
- sounds an audible warning when the air quality is unhealthful.
- stores a record of each time the air quality sensor detects contaminants, using cloud-based data storage.

## First time setup
For all the samples in this repository, see the ![General Setup Instructions](./../../README.md#setup) for required boards and libraries.  You need either Grove or DFRobot sensors but not both.

## Hardware requirements

### Grove\* 

Sensor | Pin
--- | ---
Grove\* Air Quality Sensor | AO
Grove\* Speaker | D5

### DFRobot\*

Sensor | Pin
--- | ---
Buzzer | A1
Air Quality Sensor | A3

For more specific information on the hardware requirements see ![Hardware Details](./../README.md#hardware-requirements)

## Software requirements

1. ![MRAA](https://github.com/intel-iot-devkit/mraa) and ![UPM](https://github.com/intel-iot-devkit/upm) 
2. Microsoft Azure\*, IBM Bluemix\*, AT&T M2X\*, AWS\*, Predix\*, or SAP\* account (optional)

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

After running the program you should have output similar to:
```
Grove Air Quality Sensor
Heating sensor for 3 minutes…
Please wait, 1 minute(s) passed..
Please wait, 2 minute(s) passed..
Sensor ready!
raw: 	77ppm: 	0.95	Normal Indoor Air
raw: 	40ppm: 	01.15	Fresh Air
raw: 	36ppm: 	0.98	Fresh Air
```
### Running the example with the cloud server (optional)

To run the example with the optional back-end data store, you need to set the `SERVER` and `AUTH_TOKEN` environment variables. You can do this in Eclipse as follows:

1. From the **Run** menu, select **Run Configurations**.<br> The **Run Configurations** dialog box is displayed.
2. Under **C/C++ Remote Application**, click **doorbell**.<br> This displays the information for the application.
3. In the **Commands to execute before application** field, add the environment variables so it looks like this, except using the server and authentication token that correspond to your own setup:<br>

        chmod 755 /tmp/air-quality; export SERVER="http://intel-iot-example-data.azurewebsites.net/logger/air-quality"; export AUTH_TOKEN="Enter Auth Token Here"
        
IMPORTANT NOTICE: This software is sample software. It is not designed or intended for use in any medical, life-saving or life-sustaining systems, transportation systems, nuclear systems, or for any other mission-critical application in which the failure of the system could lead to critical injury or death. The software may not be fully tested and may contain bugs or errors; it may not be intended or suitable for commercial release. No regulatory approvals for the software have been obtained, and therefore software may not be certified for use in certain countries or environments.
