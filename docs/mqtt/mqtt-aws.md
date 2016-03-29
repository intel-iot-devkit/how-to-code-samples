# Connecting to Amazon Web Services IoT using MQTT

## AWS IoT Initial Setup

- Create an account on AWS, if you do not yet have one.

- Install the AWS CLI, by following the instructions at http://docs.aws.amazon.com/cli/latest/userguide/installing.html

- Verify setup, by running this command:
```
aws iot help
```

## Create new device

To create a new device, use the `create-thing` command like this:
```
aws iot create-thing --thing-name "edison1"
```

You should receive a response such as:
```
{
    "thingArn": "arn:aws:iot:us-west-2:974066999403:thing/edison1",
    "thingName": "edison1"
}
```

## Get list of devices

To list your devices, use the `list-things` command like this:

```
aws iot list-things
```

You should receive a response such as:
```
{
    "things": [
        {
            "attributes": {},
            "thingName": "edison1"
        }
    ]
}
```

## Obtain and configure certificate for device use

- Provision a certificate
```
aws iot create-keys-and-certificate --set-as-active --certificate-pem-outfile cert.pem --public-key-outfile publicKey.pem --private-key-outfile privateKey.pem
```

You should receive a response such as:
```
{
    "certificateArn": "arn:aws:iot:us-west-2:974066999403:cert/somelongidhere",
    "certificatePem": "-----BEGIN CERTIFICATE-----\n...\n-----END CERTIFICATE-----\n",
    "keyPair": {
        "PublicKey": "-----BEGIN PUBLIC KEY-----\n...\n-----END PUBLIC KEY-----\n",
        "PrivateKey": "-----BEGIN RSA PRIVATE KEY-----\n...\n-----END RSA PRIVATE KEY-----\n"
    },
    "certificateId": "somelongidhere"
}
```

- Create/attach policy
```
aws iot create-policy --policy-name "PubSubToAnyTopic" --policy-document file:///intel/how-to-code-samples/docs/mqtt/aws-device-policy.json
```

You should receive a response such as:
```
{
    "policyName": "PubSubToAnyTopic",
    "policyArn": "arn:aws:iot:us-west-2:974066999403:policy/PubSubToAnyTopic",
    "policyDocument": "{\n    \"Version\": \"2012-10-17\",\n    \"Statement\": [{\n        \"Effect\": \"Allow\",\n        \"Action\":[\"iot:*\"],\n        \"Resource\": [\"*\"]\n    }]\n}\n",
    "policyVersionId": "1"
}
```

- Attach certificate to device
You will need the "certificate-arn" from the "Provision a certificate" step:

```
aws iot attach-principal-policy --principal "certificate-arn" --policy-name "PubSubToAnyTopic"
```



## Determine AWS endpoint

```
aws iot describe-endpoint
```

## Installing certificates to the Edison

Run the following commands from your computer:

```
scp -r cert.pem USERNAME@xxx.xxx.x.xxx:/home/root/.ssh
scp -r publicKey.pem USERNAME@xxx.xxx.x.xxx:/home/root/.ssh
scp -r privateKey.pem USERNAME@xxx.xxx.x.xxx:/home/root/.ssh
```

Note that you must change `USERNAME@xxx.xxx.x.xxx` to match whatever username and IP address that you have set your board to.

## Running code on Edison
When running the code on the Edison, you need to set the following "Commands to execute before application":

```
chmod 755 /tmp/alarm-clock; export MQTT_SERVER="ssl://A1QBI9OBPG6VY7.iot.us-west-2.amazonaws.com:8883"; export MQTT_CLIENTID="edison1"; export MQTT_CERT="/home/root/.ssh/cert.pem"; export MQTT_KEY="/home/root/.ssh/privateKey.pem"; export MQTT_CA="/etc/ssl/certs/VeriSign_Class_3_Public_Primary_Certification_Authority_-_G5.pem"; export MQTT_TOPIC="devices/edison1"
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
