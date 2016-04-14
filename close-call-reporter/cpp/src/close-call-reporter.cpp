/*
* Copyright (c) 2015-2016 Intel Corporation.
*
* Permission is hereby granted, free of charge, to any person (“User”) obtaining
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
* User understands, acknowledges, and agrees that: (i) the Software is sample software;
* (ii) the Software is not designed or intended for use in any medical, life-saving
* or life-sustaining systems, transportation systems, nuclear systems, or for any
* other mission-critical application in which the failure of the system could lead to
* critical injury or death; (iii) the Software may not be fully tested and may contain
* bugs or errors; (iv) the Software is not intended or suitable for commercial release;
* (v) no regulatory approvals for the Software have been obtained, and therefore Software
* may not be certified for use in certain countries or environments.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
* LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
* OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
* WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sstream>
#include <thread>
#include <string>
#include <signal.h>

#include <ublox6.h>
#include <rfr359f.h>

#include "../lib/restclient-cpp/include/restclient-cpp/restclient.h"

#include "datastore.h"
#include "mqtt.h"

using namespace std;

const size_t bufferLength = 256;

// The hardware devices that the example is going to connect to
struct Devices
{
  upm::RFR359F* detector;
  upm::Ublox6* nmea;

  Devices(){
  };

  // Initialization function
  void init() {
    detector = new upm::RFR359F(2);
    nmea = new upm::Ublox6(0);
    initPort();
  };

  // Cleanup on exit
  void cleanup() {
    delete detector;
    delete nmea;
  }

  int initPort(){
    if (!nmea->setupTty(B9600)) {
      cerr << "Failed to setup tty port parameters" << endl;
      return 1;
    }
    return 0;
  }

	// Get GPS data
  string gps_info(){
    string result;
    char nmeaBuffer[bufferLength];

    if (nmea->dataAvailable()) {
      int rv = nmea->readData(nmeaBuffer, bufferLength);
      if (rv > 0) {
        result = nmeaBuffer;
        return result;
      }

      if (rv < 0) {
        cerr << "Port read error." << endl;
        return "GPS Error";
      }
    }

    return "No GPS Data";
  }

	// Notify remote datastore where there is a close call
  void notify(string message, string location) {
    cout << message << endl;
    cout << location << endl;

    time_t now = time(NULL);
    char mbstr[sizeof "2011-10-08T07:07:09Z"];
    strftime(mbstr, sizeof(mbstr), "%FT%TZ", localtime(&now));

    stringstream text;
    text << "{";
    text << "\"message\":";
    text << "\"" << message << " " << mbstr << "\"";
    text << "\"location\":";
    text << "\"" << location << "\"";
    text << "}";

    log_mqtt(text.str());
    log_datastore(text.str());
  }

	// Check to see if any object is within range
  void check_object_detected(){
    if (detector->objectDetected()) {
      notify("object-detected", gps_info());
    } else {
      cout << "Area is clear" << endl;
    }
  }
};

Devices devices;

// Exit handler for program
void exit_handler(int param)
{
  devices.cleanup();
  exit(1);
}

// The main function for the example program
int main() {
  // Handles ctrl-c or other orderly exits
  signal(SIGINT, exit_handler);

  // check that we are running on Galileo or Edison
  mraa_platform_t platform = mraa_get_platform_type();
  if ((platform != MRAA_INTEL_GALILEO_GEN1) &&
    (platform != MRAA_INTEL_GALILEO_GEN2) &&
    (platform != MRAA_INTEL_EDISON_FAB_C)) {
    cerr << "ERROR: Unsupported platform" << endl;
    return MRAA_ERROR_INVALID_PLATFORM;
  }

  // create and initialize UPM devices
  devices.init();

	for (;;) {
    devices.check_object_detected();
    sleep(5);
  }

  return MRAA_SUCCESS;
}
