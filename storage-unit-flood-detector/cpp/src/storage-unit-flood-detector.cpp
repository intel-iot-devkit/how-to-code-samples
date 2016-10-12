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
 * @brief Storage unit flood detector in C++
 *
 * This storage-unit-flood-detector application is part of a series of how-to Intel IoT code
 * sample exercises using the Intel® IoT Developer Kit, Intel® Edison board,
 * cloud platforms, APIs, and other technologies.
 *
 * @hardware Sensors used:\n
 * Grove Speaker\n
 * Grove Mousture Sensor\n
 *
 * @cc
 * @cxx -std=c++1y
 * @ld -lupm-grovespeaker -lupm-grovemoisture -lpaho-mqtt3cs -lcurl
 *
 * Additional source files required to build this example:
 * @req datastore.cpp
 * @req mqtt.cpp
 *
 * @date 09/22/2016
 */

#include <stdlib.h>
#include <iostream>
#include <signal.h>
#include <ctime>
#include <sstream>

using namespace std;

#include "kits.h"
#if INTEL_IOT_KIT == DFROBOTKIT
#include "dfrobotkit.hpp"
#else
#include "grovekit.hpp"
#endif

#include "../lib/restclient-cpp/include/restclient-cpp/restclient.h"

#include "datastore.h"
#include "mqtt.h"

// Send notification to remote datastore
void notify() {
  time_t now = time(NULL);
  char mbstr[sizeof "2011-10-08T07:07:09Z"];
  strftime(mbstr, sizeof(mbstr), "%FT%TZ", localtime(&now));

  stringstream text;
  text << "{\"value\":";
  text << "\"" << mbstr << "\"}";

  log_mqtt(text.str());
  log_datastore(text.str());
}

Devices devices;

// Starts the alarm
void alarm() {
  cout << "Alert! Water is Detected!";
  devices.alarm();
  notify();
}

// Every 1 second, reads the moisture sensor
void sense_moisture() {
  for (;;) {
    int val = devices.readMoisture();
    cout << "Moisture value: " << val << ", ";
    if (val >= 0 && val < 300)
        cout << "dry";
    else if (val >= 300 && val < 600)
        cout << "moist";
    else
        cout << "wet";

    cout << endl;

    if (val >= 300)
       alarm();

    cout << endl;
    sleep(1);
  }
}

// Exit handler for program
void exit_handler(int param)
{
  devices.cleanup();
  exit(1);
}

// The main function for the example program
int main()
{
  // handles ctrl-c or other orderly exits
  signal(SIGINT, exit_handler);

  // create and initialize UPM devices
  devices.init();
  sense_moisture();

  return 0;
}
