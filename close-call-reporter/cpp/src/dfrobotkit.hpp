/*
* Copyright (c) 2015 - 2016 Intel Corporation.
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
* LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
* OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
* WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef DFROBOTKIT_HPP_
#define DFROBOTKIT_HPP_

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sstream>
#include <thread>
#include <string>
#include <signal.h>

#include <nmea_gps.hpp>
#include <rfr359f.hpp>

using namespace std;

// The hardware devices that the example is going to connect to
struct Devices
{
  upm::RFR359F* detector;
  upm::NMEAGPS *nmea;

  Devices(){
  };

  // Initialization function
  void init() {
    detector = new upm::RFR359F(2);
    nmea = new upm::NMEAGPS(0, 38400, -1);
  };

  // Cleanup on exit
  void cleanup() {
    delete detector;
    delete nmea;
  }

	// Get GPS data
  string gps_info(){
    size_t bufferLength = 256;
    string result;

    if (nmea->dataAvailable(bufferLength)) {
      string rv = nmea->readStr(bufferLength);
      if (rv.length() > 0) {
        return rv;
      }

      if (rv.length() < 0) {
        cerr << "Port read error." << endl;
        return "GPS Error";
      }
    }

    return "No GPS Data";
  }

	// Check to see if any object is within range
  bool objectDetected(){
    return detector->objectDetected();
  }
};

#endif /* DFROBOTKIT_HPP_ */
