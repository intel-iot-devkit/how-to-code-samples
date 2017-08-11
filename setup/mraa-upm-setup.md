##Check if already installed
Your OS may have MRAA and UPM already installed.  Here's are the various ways you can check if it's already installed and the version number.
1. If you're running Ubuntu, enter:

	$ dpkg -L libmraa-dev
	
If you get 'dpkg-query: package 'libmraa-dev' is not installed, then proceed to install it using the apt-get method below.

2. Check the 
3.

##Check version number
		For UPM: https://github.com/intel-iot-devkit/upm/releases 
		For MRAA: https://github.com/intel-iot-devkit/mraa/releases 
		
##Installing using apt-get

	$ sudo add-apt-repository ppa:mraa/mraa
	$ sudo apt-get update
	$ sudo apt-get install libmraa1 libmraa-dev mraa-tools python-mraa python3-mraa

##Installing using npm

#Installing using rpm

##Flashing Arduino 101 with Firmata sketch

##How to verify Arduino 101/imraa setup



