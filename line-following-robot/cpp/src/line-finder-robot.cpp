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
 * @brief Line following robot in C++
 *
 * This line-finder-robot application is part of a series of how-to Intel IoT code
 * sample exercises using the Intel® IoT Developer Kit, Intel® Edison board,
 * cloud platforms, APIs, and other technologies.
 *
 * @hardware Sensors used:\n
 * Grove Line Finder\n
 * ULN200XA Stepper Driver x2\n
 * 28BYJ-48 Unipolar Stepper Motor x2\n
 *
 * @cc
 * @cxx -std=c++1y
 * @ld -lupm-grovelinefinder -lupm-uln200xa -lpaho-mqtt3cs -lupm-grove -lcurl
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
#include <signal.h>

#include <grovelinefinder.hpp>
#include <uln200xa.hpp>

#include "../lib/restclient-cpp/include/restclient-cpp/restclient.h"

#include "datastore.h"
#include "mqtt.h"

using namespace std;

// Notify remote datastore
void notify(std::string message) {

  std::time_t now = std::time(NULL);
  char mbstr[sizeof "2011-10-08T07:07:09Z"];
  std::strftime(mbstr, sizeof(mbstr), "%FT%TZ", std::localtime(&now));

  std::stringstream text;
  text << "{\"state\":";
  text << "\"" << message << " " << mbstr << "\"}";

  log_mqtt(text.str());
  log_datastore(text.str());
}

// The hardware devices that the example is going to connect to
struct Devices
{
  upm::GroveLineFinder* finder;
  upm::ULN200XA* stepLeft;
  upm::ULN200XA* stepRight;

  Devices(){
  };

  // Initialization function
  void init() {
    // line finder connected to d2
    finder = new upm::GroveLineFinder(2);

    // left stepper motor connected to d9,10,11,12
    stepLeft = new upm::ULN200XA(4096, 9, 10, 11, 12);

    // right stepper motor connected to 4, 5, 6, 7
    stepRight = new upm::ULN200XA(4096, 4, 5, 6, 7);
  };

  // Cleanup on exit
  void cleanup() {
    delete finder;

    stepLeft->release();
    stepRight->release();

    delete stepLeft;
    delete stepRight;
  }

  // Function to make robot find and follow a black line
  void findFollowLine(){
    for (;;)
    {
      bool val = finder->blackDetected();
      std::cout << "Line detected: " << val << std::endl;

      if (val) {
        moveForward();
        notify("Moving on line. \n");
      }
      else
        pivotClockwise();
    }
  }

  // Have both motors move forward, assuming motors
  // are on opposite sides of each other
  void moveForward(){
    stepLeft->setSpeed(5);
    stepRight->setSpeed(5);

    stepLeft->setDirection(upm::ULN200XA::DIR_CW);
    stepRight->setDirection(upm::ULN200XA::DIR_CW);

    stepLeft->stepperSteps(1024);
    stepRight->stepperSteps(1024);
  }

  // Have both motors pivot by one moving clockwise,
  // and the other counterclockwise, assuming motors
  // are on opposite sides of each other
  void pivotClockwise(){
    stepLeft->setSpeed(5);
    stepRight->setSpeed(5);

    stepLeft->setDirection(upm::ULN200XA::DIR_CW);
    stepRight->setDirection(upm::ULN200XA::DIR_CCW);

    stepLeft->stepperSteps(512);
    stepRight->stepperSteps(512);

    sleep(1);
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

  for(;;) {
    devices.findFollowLine();
  }

  return MRAA_SUCCESS;
}
