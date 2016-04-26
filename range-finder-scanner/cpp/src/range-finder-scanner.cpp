/*
* Copyright (c) 2015-2016 Intel Corporation.
*
* Permission is hereby granted, free of charge, to any person ("User") obtaining
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
 * @brief Range finder scanner in C++
 *
 * This range-finder-scanner application is part of a series of how-to Intel IoT code
 * sample exercises using the Intel® IoT Developer Kit, Intel® Edison board,
 * cloud platforms, APIs, and other technologies.
 *
 * @hardware Sensors used:\n
 * Grove Distance Interrupter\n
 * ULN200XA Stepper Driver\n
 * 28BYJ-48 Unipolar Stepper Motor\n
 *
 * @cc
 * @cxx -std=c++1y
 * @ld -upm-uln200xa -lupm-rfr359f -lpaho-mqtt3cs -lupm-grove -lboost_system -lboost_thread -lboost_filesystem -lboost_date_time -lpthread -lcurl
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
#include <array>
#include <string>

#include <uln200xa.h>
#include <rfr359f.h>

#include "../lib/crow/crow_all.h"
#include "html.h"
#include "styles.h"

bool degrees[360];

// The hardware devices that the example is going to connect to
struct Devices
{
  upm::RFR359F* interuptor;
  upm::ULN200XA* stepper;

  Devices(){
  };

  // Initialization function
  void init() {
    // range finder connected to d2
    interuptor = new upm::RFR359F(2);

    // stepper motor connected to d9,10,11,12
    stepper = new upm::ULN200XA(4096, 9, 10, 11, 12);

    for (int i = 0; i < 360; i++){
      degrees[i] = false;
    }
  };

  // Cleanup on exit
  void cleanup() {
    delete interuptor;
    stepper->release();
    delete stepper;
  }

  // Moves the stepper motor around and around
  void move(){
    stepper->setDirection(upm::ULN200XA::DIR_CW);
    stepper->setSpeed(5);
    stepper->stepperSteps(4096/360);
  }

  // Check to see if any object has been detected
  void check_object_detected(){
    for(int i = 0; i<360; i++){
      bool isDetected = interuptor->objectDetected();
      if (isDetected){
        std::cout << "Object detected" << std::endl;
        degrees[i] = true;
      }
      else{
        std::cout << "Area is clear" << std::endl;
        degrees[i] = false;
      }
      move();
      sleep(1);
    }
  }
};

// Function called by worker thread for device communication
void runner(Devices& devices) {
  for (;;) {
    devices.check_object_detected();
  }
}

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

  // start worker thread for device communication
  std::thread t1(runner, std::ref(devices));

  // define web server & routes
  crow::SimpleApp app;

  CROW_ROUTE(app, "/")
  ([]() {
    std::stringstream text;
    text << index_html;
    return text.str();
  });

  CROW_ROUTE(app, "/data.json")
  ([](const crow::request& req) {
    crow::json::wvalue result;
    for (int i = 0; i < 360; i++) {
      result[i] = degrees[i];
    }

    return crow::response{result};
  });

  CROW_ROUTE(app, "/styles.css")
  ([]() {
    std::stringstream text;
    text << styles_css;
    return text.str();
  });


  // start web server
  app.port(3000).multithreaded().run();

  // wait forever for the thread to exit
  t1.join();

  return MRAA_SUCCESS;
}
