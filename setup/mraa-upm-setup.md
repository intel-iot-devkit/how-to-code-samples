##Verifying your mraa installation

If you want to check whether your installation of mraa was successful, or if you're not sure if mraa is already installed, there are several methods available for verification.

Important note:  mraa is built in C with SWIG bindings to C++, Python, Java, and Node.js.  Installation in one doesn't guarantee installation in any of the others.  In particular, Node.js bindings can only be installed through the node package manager (npm).

Important note: upm, a sensor library which is dependent on mraa to operate, has a separate installation procedure (https://github.com/intel-iot-devkit/upm/blob/master/docs/installing.md) once mraa has been installed.

1. If you're running Ubuntu, enter:

	$ dpkg -L libmraa-dev
	
If you get 'dpkg-query: package 'libmraa-dev' is not installed, then it is not installed.  If you see a list of directories then the installation was successful.

2. Run the mraa-gpio version command from any prompt
After mraa is installed, a command line tool, called 'mraa-gpio' is made available.  You can use it to manually exercise gpio pins, or to just check the mraa version number
	$ mraa-gpio version
	
If you get 'mraa-gpio' command not found, then it isn't installed.

3. Check the /usr/include directory for mraa.h as well as an mraa folder.  If there isn't one then it probably wasn't installed.

**Editor's note**: This was checked on Ubuntu desktop, need to check NUC gateway with WR linux as well

##Tests for each language binding

###Python:

	$ python
	>>> import mraa

If that doesn't result in an error then the Python bindings are present.  

###Node.js

	$ node
	> var m = require('mraa')
	
If you get 'Error: Cannot find module 'mraa' ' , then the Node.js bindings haven't been installed.  If you've installed mraa using apt-get and the ppa package, then the Node.js bindings will not be included. See the section below*** on Installing using npm.

###C++

Create a blank application and include the mraa.hpp header file.

```C++
#include "mraa.hpp"
```

If it compiles *on the target platform* then it is installed correctly.

###Java

See this example https://github.com/intel-iot-devkit/mraa/blob/master/examples/java/Example.java 

##Check version number
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



