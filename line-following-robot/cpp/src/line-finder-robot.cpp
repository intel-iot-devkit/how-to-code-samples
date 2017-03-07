/*
* Copyright (c) 2015 - 2017 Intel Corporation.
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
 * @date 02/13/2017
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

// Notify remote datastoare
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

  int linePin = 2,
      stepLeftInputPin1 = 9,
      stepLeftInputPin2 = 10,
      stepLeftInputPin3 = 11,
      stepLeftInputPin4 = 12,
      stepRightInputPin1 = 4,
      stepRightInputPin2 = 5,
      stepRightInputPin3 = 6,
      stepRightInputPin4 = 7;

  Devices(){
  };

  // Set pins/init as needed for specific platforms
  void set_pins() {
    mraa_platform_t platform = mraa_get_platform_type();
    switch (platform) {
      case MRAA_INTEL_GALILEO_GEN1:
      case MRAA_INTEL_GALILEO_GEN2:
      case MRAA_INTEL_EDISON_FAB_C:
         break;
      case MRAA_GENERIC_FIRMATA:
         linePin += 512;
         stepLeftInputPin1 += 512;
         stepLeftInputPin2 += 512;
         stepLeftInputPin3 += 512;
         stepLeftInputPin4 += 512;
         stepRightInputPin1 += 512;
         stepRightInputPin2 += 512;
         stepRightInputPin3 += 512;
         stepRightInputPin4 += 512;

         break;
      default:
        // try using firmata
        string port = "/dev/ttyACM0";
        if (getenv("PORT"))
        {
          port = getenv("PORT");
        }
        mraa_result_t res = mraa_add_subplatform(MRAA_GENERIC_FIRMATA, port.c_str());
        if (res != MRAA_SUCCESS){
          std::cerr << "ERROR: Base platform " << platform << " on port " << port.c_str() << " for reason " << res << std::endl;
        }
        linePin += 512;
        stepLeftInputPin1 += 512;
        stepLeftInputPin2 += 512;
        stepLeftInputPin3 += 512;
        stepLeftInputPin4 += 512;
        stepRightInputPin1 += 512;
        stepRightInputPin2 += 512;
        stepRightInputPin3 += 512;
        stepRightInputPin4 += 512;
    }
  }

  // Initialization function
  void init() {
    set_pins();

    // line finder connected to d2
    finder = new upm::GroveLineFinder(linePin);

    // left stepper motor connected to d9,10,11,12
    stepLeft = new upm::ULN200XA(4096, stepLeftInputPin1, stepLeftInputPin2, stepLeftInputPin3, stepLeftInputPin4);

    // right stepper motor connected to 4, 5, 6, 7
    stepRight = new upm::ULN200XA(4096, stepRightInputPin1, stepRightInputPin2, stepRightInputPin3, stepRightInputPin4);
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
        std::cout << "Moving on line..." << std::endl;
        moveForward();
        notify("moving\n");
      }
      else
        std::cout << "Pivoting..." << std::endl;
        pivotClockwise();
    }
  }

  // Have both motors move forward, assuming motors
  // are on opposite sides of each other
  void moveForward(){
    stepLeft->setSpeed(5);
    stepRight->setSpeed(5);

    stepLeft->setDirection(ULN200XA_DIR_CW);
    stepRight->setDirection(ULN200XA_DIR_CW);

    stepLeft->stepperSteps(1024);
    stepRight->stepperSteps(1024);
  }

  // Have both motors pivot by one moving clockwise,
  // and the other counterclockwise, assuming motors
  // are on opposite sides of each other
  void pivotClockwise(){
    stepLeft->setSpeed(5);
    stepRight->setSpeed(5);

    stepLeft->setDirection(ULN200XA_DIR_CW);
    stepRight->setDirection(ULN200XA_DIR_CCW);

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

  // create and initialize UPM devices
  devices.init();

  for(;;) {
    devices.findFollowLine();
  }

  return MRAA_SUCCESS;
}
