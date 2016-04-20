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

/**
 * @file
 * @ingroup howtocode
 * @brief Air quality sensor in C++
 *
 * This air-quality-sensor application is part of a series of how-to Intel IoT code
 * sample exercises using the IntelÂ® IoT Developer Kit, IntelÂ® Edison board,
 * cloud platforms, APIs, and other technologies.
 *
 * @hardware Sensors used:\n
 * Grove Air Quality Sensor\n
 * Grove Speaker\n
 *
 * @cc
 * @cxx -std=c++1y
 * @ld -lupm-grove-speaker -lupm-gas -lpaho-mqtt3cs -lupm-grove -lboost_system -lboost_thread -lboost_filesystem -lboost_date_time -lpthread -lcurl
 *
 * Additional source files required to build this example:
 * @req datastore.cpp
 * @req mqtt.cpp
 *
 * @date 04/04/2016
 */

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sstream>
#include <thread>
#include <ctime>
#include <string>
#include <signal.h>

#include <grovespeaker.h>
#include "tp401.h"

#define WARNING_THRESHOLD 200

#include "../lib/restclient-cpp/include/restclient-cpp/restclient.h"

#include "datastore.h"
#include "mqtt.h"

using namespace std;

struct Devices
{
  upm::GroveSpeaker* speaker;
  upm::TP401* sensor;

  Devices() {
  };

  // Initialization function
  void init() {
  	// speaker connected to digital D5
    speaker = new upm::GroveSpeaker(5);

    // air sensor connected to analog A0
    sensor = new upm::TP401(0);

    // start sensor warmup process
    warmup();
  };

  // Sounds an audible alarm
  void alarm() {
    speaker->playSound('a', true, "high");
    speaker->playSound('c', true, "high");
    speaker->playSound('g', true, "high");
  }

  // Notify the remote datastore
  void notify() {

    std::time_t now = std::time(NULL);
    char mbstr[sizeof "2011-10-08T07:07:09Z"];
    std::strftime(mbstr, sizeof(mbstr), "%FT%TZ", std::localtime(&now));

    std::stringstream text;
    text << "{\"value\":";
    text << "\"" << mbstr << "\"}";

    log_mqtt(text.str());
    log_datastore(text.str());
  }

  // Cleanup on exit
  void cleanup() {
    delete speaker;
    delete sensor;
  }

  // How is the air quality?
  string air_quality(uint16_t value)
  {
    if(value < 30) return "Fresh Air";
    if(value < 100) return "Normal Indoor Air";
    if(value < 200) return "Low Pollution";
    if(value < 300) return "High Pollution - Action Recommended";
    return "Very High Pollution - Take Action Immediately";
  }

  // Warmup the air quality sensor for 3 minutes
  void warmup(){
    cout << sensor->name() << endl;

    fprintf(stdout, "Heating sensor for 3 minutes...\n");

    // wait 3 minutes for sensor to warm up
    for(int i = 0; i < 3; i++) {
      if(i) {
        fprintf(stdout, "Please wait, %d minute(s) passed..\n", i);
      }
      sleep(60);
    }

    fprintf(stdout, "Sensor ready!\n");
  }

  // Check the air qulity by reading the sensor
  void check_air_quality(){
    // read raw value
    uint16_t value = sensor->getSample();

    // read CO ppm (can vary slightly from previous read)
    float ppm = sensor->getPPM();
    fprintf(stdout, "raw: %4d ppm: %5.2f   %s\n", value, ppm, air_quality(value).c_str());

    if (value > WARNING_THRESHOLD){
    	notify();
    	alarm();
    }

    // sleep for 2.5s
    usleep(2500000);
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
    std::cerr << "ERROR: Unsupported platform" << std::endl;
    return MRAA_ERROR_INVALID_PLATFORM;
  }

  // create and initialize UPM devices
  devices.init();

  for (;;) {
  	devices.check_air_quality();
  }

  return MRAA_SUCCESS;
}
