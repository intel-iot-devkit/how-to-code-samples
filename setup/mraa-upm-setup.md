##Check if MRAA is already installed
Your OS may have MRAA and UPM already installed.  Since MRAA is a dependency of UPM, first check if MRAA is installed.

1. If you're running Ubuntu, enter:

	$ dpkg -L libmraa-dev
	
If you get 'dpkg-query: package 'libmraa-dev' is not installed, then proceed to install it using the apt-get method below.  If you see a list of directories where mraa was installed, then it is successful.

2. Run the mraa-gpio version command from any prompt
After mraa is installed, a command line tool, called 'mraa-gpio' is made available.  You can use it to manually exercise gpio pins, or to just check the mraa version number
	$ mraa-gpio version
	
If you get 'mraa-gpio' command not found, then it isn't installed.

*Important Note: Just because you have mraa installed doesn't mean you have UPM, the sensor library, installed as well. See the next section on how to check if UPM is installed.

3. Check the /usr/include directory for mraa.h as well as an mraa folder.  If there isn't one then it probably wasn't installed.

**Editor's note**: This was checked on Ubuntu desktop, need to check NUC gateway with WR linux as well

4. Run a simple test in Python:

	$ python
	>>> import mraa

If that doesn't result in an error then the Python bindings are present.  You can run a similar test for Node.js

	$ node
	> var m = require('mraa')
	
If you get 'Error: Cannot find module 'mraa' ' , then the Node.js bindings haven't been installed.  If you've installed mraa using apt-get and the ppa package, then the Node.js bindings will not be included. See the section below on Installing using npm.

##Check version number
		For UPM: https://github.com/intel-iot-devkit/upm/releases 
		For MRAA: https://github.com/intel-iot-devkit/mraa/releases 
		
##Installing using apt-get (For Ubuntu Desktop or Server)

	$ sudo add-apt-repository ppa:mraa/mraa
	$ sudo apt-get update
	$ sudo apt-get install libmraa1 libmraa-dev mraa-tools python-mraa python3-mraa

##Installing using npm

	$ npm install mraa

For more details on how this installation differs see this section on Installing for Node.js only (https://github.com/intel-iot-devkit/mraa#installing-for-nodejs-only) 

#Installing using rpm

##Flashing Arduino 101 with Firmata sketch

##How to verify Arduino 101/imraa setup



