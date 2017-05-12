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

/**
 * @file
 * @ingroup howtocode
 * @brief Close call reporter in C++
 *
 * This close-call-reporter application is part of a series of how-to Intel IoT code
 * sample exercises using the Intel® IoT Developer Kit, Intel® Edison board,
 * cloud platforms, APIs, and other technologies.
 *
 * @hardware Sensors used:\n
 * Grove Distance Interrupter\n
 * Grove GPS\n
 *
 * @cc
 * @cxx -std=c++1y
 * @ld -lupm-rfr359f -lupm-ublox6 -lpaho-mqtt3cs -lupm-grove -lboost_system -lboost_thread -lboost_filesystem -lboost_date_time -lpthread -lcurl
 *
 * Additional source files required to build this example:
 * @req datastore.cpp
 * @req mqtt.cpp
 *
 * @date 09/07/2016
 */
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sstream>
#include <thread>
#include <string>
#include <signal.h>

#include "kits.h"
#if INTEL_IOT_KIT == DFROBOTKIT
#include "dfrobotkit.hpp"
#else
#include "grovekit.hpp"
#endif

#include "../lib/restclient-cpp/include/restclient-cpp/restclient.h"

#include "datastore.h"
#include "services/services.h"

using namespace std;

Devices devices;

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

  log_service(text.str());
  log_datastore(text.str());
}

// Check to see if any object is within range
void check_object_detected(){
  if (devices.objectDetected()) {
    notify("object-detected", devices.gps_info());
  } else {
    cout << "Area is clear" << endl;
  }
}

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
    check_object_detected();
    sleep(5);
  }

  return MRAA_SUCCESS;
}
