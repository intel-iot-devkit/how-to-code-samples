#How-to Intel code samples

These applications are how-to code samples using the Intel® IoT Developer Kit and compatible Intel® IoT Platforms, cloud platforms, APIs, and other technologies. 

## To clone this repository

To begin, clone the **How-To Code Samples** repository with Git\* on your computer as follows:

    $ git clone https://github.com/intel-iot-devkit/how-to-code-samples.git

To download a .zip file, in your web browser go to <a href="https://github.com/intel-iot-devkit/how-to-code-samples">https://github.com/intel-iot-devkit/how-to-code-samples</a> and click the **Download ZIP** button at the lower right. Once the .zip file is downloaded, uncompress it, and then use the files in the directory for this example.

### Installing the program manually on the target system

Alternatively, you can set up the code manually on your system.

Clone the **How-To Code Samples** repository to your board:

    $ git clone https://github.com/intel-iot-devkit/how-to-code-samples.git

Navigate to the directory with this example.

# Setup

## Hardware Compatibility

These samples are currently supported by the following platforms:

Intel IoT Gateway using Arduino 101 (https://software.intel.com/en-us/iot/hardware/gateways) 

## Software Requirements

### mraa

[mraa.io](mraa.io)

### upm

[upm.io](upm.io)

### Integrated Development Environments (IDEs)

For Java or C/C++ development, use the Intel System Studio IoT Edition - https://software.intel.com/en-us/iot/tools-ide/ide/iss-iot-edition 

For JavaScript development, use the Intel® XDK - https://software.intel.com/en-us/intel-xdk

## Configuring the examples

Each example includes a configuration file to setup sensors, cloud credentials, and other services.

For C++ look in cpp.json
For Java look in /src/config.properties
For JavaScript look in config.json
For Python look in config.py

Refer to each example for the specific configuration needed.

## Cloud and datstore configuration

Specific instructions on setting up individual cloud services can be found here https://github.com/intel-iot-devkit/iot-samples-cloud-setup

Adding a Redis\* datastore can be done by following instructions here https://github.com/intel-iot-devkit/intel-iot-examples-datastore 

## Finding your board's IP address

### On Wind River Linux

### On Ubuntu

##

