# Connecting to Microsoft Azure IoT Hub using MQTT

## MS Azure Initial Setup

- Create an account on Microsoft Azure, if you do not yet have one.
- Login to your Microsoft Azure account.
- Create a new IoT Hub by clicking on the "New" link on your Dashboard. Click on the "Internet of Things" links, then click on the "Azure IoT Hub" link.
- Enter the information for your new Azure IoT Hub, then click on the "Create" button.
- Your new Azure IoT Hub will be created in just a few moments.

## Obtain SAS token for administrative use

Once your Azure IoT Hub has been created, you need to obtain a SharedAccessSignature (SAS) token to perform adminitrativee actions such as creating or listing devices.

- Go to your Dashboard, then click on the link to your new Azure IoT Hub resource.
- Click on "Settings", then click on "Shared Access Policies".
- Obtain the ***host*** name and the ***primary key*** for the ***registryReadWrite*** policy.
- Create an SharedAccessSignature (SAS) token to be used to create and list devices. You can use the `sastoken` command line program like this:

```
sastoken <Your IoT Hub Name>.azure-devices.net/devices/ <Your primary key> 1440 registryReadWrite
```

The program will output your SAS token as a string like this:

```
SharedAccessSignature sr=<Your IoT Hub Name>.azure-devices.net%2Fdevices&sig=<Super secret code>&se=<expiration timestamp>&skn=registryReadWrite
```

This SAS token will last for 1440 minutes before you will need to obtain a new one. It should be used only for administrative functions such as creating new devices. Each device will also need its own SAS token, which we will create in a subsequent step.

## Create new device

You can use ***curl*** to create a new device for your Azure IoT Hub using the SAS token with ***registryReadWrite*** access as obtained above. For example, to create a new device named "edison1":

```
$ curl -i -X PUT -H "Content-Type: application/json" -H "Authorization: <Your SharedAccessSignature>" -d "{deviceId: \"edison1\"}" https://<Your IoT Hub Name>.azure-devices.net/devices/edison1?api-version=2016-02-03
```

You should receive a response like this:

```
HTTP/1.1 200 OK
Content-Length: 466
Content-Type: application/json; charset=utf-8
ETag: "MA=="
Server: Microsoft-HTTPAPI/2.0
Date: Thu, 24 Mar 2016 20:30:33 GMT

{"deviceId":"edison1","generationId":"635944482332920305","etag":"MA==","connectionState":"Disconnected","status":"enabled","statusReason":null,"connectionStateUpdatedTime":"0001-01-01T00:00:00","statusUpdatedTime":"0001-01-01T00:00:00","lastActivityTime":"0001-01-01T00:00:00","cloudToDeviceMessageCount":0,"authentication":{"symmetricKey":{"primaryKey":"<device primary key here>","secondaryKey":"<device secondary key here>"}}}
```

## Get list of devices

You can use ***curl*** to get the list of current devices for your Azure IoT Hub using the SAS token with ***registryReadWrite*** access as obtained above.

```
curl -i -H "Accept: application/json" -H "Authorization: <Your SharedAccessSignature>" https://<Your IoT Hub Name>.azure-devices.net/devices?api-version=2016-02-03
```

You should receive a response like this:

```
HTTP/1.1 200 OK
Content-Length: 473
Content-Type: application/json; charset=utf-8
Server: Microsoft-HTTPAPI/2.0
Date: Fri, 25 Mar 2016 20:11:58 GMT

[{"deviceId":"edison1","generationId":"635944482332920305","etag":"MA==","connectionState":"Connected","status":"enabled","statusReason":null,"connectionStateUpdatedTime":"2016-03-25T05:28:11.4588528","statusUpdatedTime":"0001-01-01T00:00:00","lastActivityTime":"0001-01-01T00:00:00","cloudToDeviceMessageCount":0,"authentication":{"symmetricKey":{"primaryKey":"<device primary key here>","secondaryKey":"<device secondary key here>"}}}]
```
You will need the device's ***primaryKey*** or ***secondaryKey*** to obtain an SAS token for the device itself to connect to the Azure IoT Hub on its own.

## Obtain SAS token for device use

You need to create an SAS token for the Intel Edison to be able to connect to the Azure IotHub to record data. The SAS token for this purpose will have less priviliages then the SAS token created for admin use. You can use the `sastoken` command line program like this:

```
sastoken <Your IoT Hub Name>.azure-devices.net/devices/<Your device name> <Your device primary key> 1440
```

The program will output the SAS token for your device as a string like this:

```
SharedAccessSignature sr=<Your IoT Hub Name>.azure-devices.net%2Fdevices%2F<Your device name>&sig=<Super secret code>&se=<expiration timestamp>
```

This SAS token will last for 1440 minutes before you will need to obtain a new one. It should be used only for the specific device for which it was created. In other words, each device that you wish to connect will need its own SAS token.

## Running code on Edison
When running the code on the Edison, you need the following "Commands to execute before application":

```
chmod 755 /tmp/alarm-clock; export MQTT_SERVER="ssl://<Your IoT Hub Name>.azure-devices.net:8883"; export MQTT_CLIENTID="<Your Device Name>"; export MQTT_USERNAME="<Your IoT Hub Name>.azure-devices.net/<Your Device Name>"; export MQTT_PASSWORD="<Your SharedAccessSignature>";
```

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
