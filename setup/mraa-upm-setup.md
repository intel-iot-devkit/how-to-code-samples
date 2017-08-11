##Check if already installed
Your OS may have MRAA and UPM already installed.  Here's are the various ways you can check if it's already installed and the version number.
1. If you're running Ubuntu, enter:

	$ dpkg -L libmraa-dev
	
If you get 'dpkg-query: package 'libmraa-dev' is not installed, then proceed to install it using the apt-get method below.  If you see a list of directories where mraa was installed, then it is successful.

2. Run the mraa-gpio version command from any prompt
After mraa is installed, a command line tool, called 'mraa-gpio' is made available.  You can use it to manually exercise gpio pins, or to just check the mraa version number
	$ mraa-gpio version
If you get 'mraa-gpio' command not found, then it isn't installed

*Important Note: Just because you have mraa installed doesn't mean you have UPM, the sensor library, installed as well. See #4 on how to check if UPM is installed.

3. Check the /

##Check version number
		For UPM: https://github.com/intel-iot-devkit/upm/releases 
		For MRAA: https://github.com/intel-iot-devkit/mraa/releases 
		
##Installing using apt-get (For Ubuntu Desktop or Server)

	$ sudo add-apt-repository ppa:mraa/mraa
	$ sudo apt-get update
	$ sudo apt-get install libmraa1 libmraa-dev mraa-tools python-mraa python3-mraa

##Installing using npm

#Installing using rpm

##Flashing Arduino 101 with Firmata sketch

##How to verify Arduino 101/imraa setup



