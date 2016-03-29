# Connecting to IBM Bluemix Internet of Things Platform using MQTT

## IBM Bluemix IoT Platform Initial Setup

- Create an account on IBM Bluemix, if you do not yet have one.
- Login to your account
- Choose "New Dashboard" if that option is presented

## Add Internet of Things Platform

- Go to the "Dashboard"
- Click on "+" to add a new service
- Click on "Internet of Things Platform"
- Enter "Service name"
- Choose a "Pricing Plan"
- Click on the "Create" button

## Add a device type

- Under "Connect your devices" click on the "Launch Dashboard" button. This will open a new window with your "IBM Watson IoT Platform" Dashboard.
- Click on the "Add Device" button.
- Click on the "Create device type" button. This will display the "Create Device Type" page.
- Click on the "Create device type" button.
- Enter the "Name" and "Description", then click on the "Next" button.
- Define any attributes for your template, then click on the "Next" button.
- On the "Submit Information" page, click on the "Next" button.
- On the "Metadata" page, click on the "Create" button.

## Add a device

- Select the new device type from the dropdown that you created in the previous step, then click on "Next".
- On the "Add Device" page, enter the "Device ID", then click on the "Next" button.
- On the "Metadata" page, click on the "Create" button.
- On the "Security" page, auto-generate an authentication token by just clicking on the "Next" button.
- On the "Summary" page, click on the "Add" button.
- Make a note of the "Authentication Token" displayed on the "Your Device Credentials" page. Please note that "Authentication tokens are non-recoverable. If you misplace this token, you will need to re-register the device to generate a new authentication token"

## Running code on Edison
When running the code on the Edison, you need to set the following "Commands to execute before application":

```
chmod 755 /tmp/alarm-clock; export MQTT_SERVER="ssl://ovusxw.messaging.internetofthings.ibmcloud.com:8883"; export MQTT_CLIENTID="d:ovusxw:edison:edison1"; export MQTT_USERNAME="use-token-auth"; export MQTT_PASSWORD="ABC1234"; export MQTT_TOPIC="iot-2/evt/alarm-clock/fmt/json"
```

Click on the "Apply" button too save these settings.

Click on the "Run" button to run the code on the Edison.

## Updating the MQTT C client library on the Edison
In order to use MQTT with the "How To Code" samples, you will need to update the Paho MQTT C client libs on the Edison. To do this, SSH into the Edison, and then run the following commands from the Edison itself:

```
opkg install coreutils
opkg remove paho-mqtt-dev
opkg remove paho-mqtt --force-depends
git clone https://github.com/eclipse/paho.mqtt.c.git
cd paho.mqtt.c
export prefix=/usr; make install
```

Then logout from the SSH session, and copy the include files and compiled libs back from the Edison to the cross compiler on your local machine like this:
```
scp -r USERNAME@xxx.xxx.x.xxx:/usr/include/MQTT* ~/Downloads/iotdk-ide-linux/devkit-x86/sysroots/i586-poky-linux/usr/include
scp USERNAME@xxx.xxx.x.xxx:/usr/lib/libpaho-mqtt* ~/Downloads/iotdk-ide-linux/devkit-x86/sysroots/i586-poky-linux/usr/lib
```

Note that you must change `USERNAME@xxx.xxx.x.xxx` to match whatever username and IP address you set your board to. ALso, change `~/Downloads/iotdk-ide-linux` to match the location on your computer where you installed the Intel® IoT Developer Kit.
