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
 * @brief Robot arm in C++
 *
 * This robot-arm application is part of a series of how-to Intel IoT code
 * sample exercises using the IntelÂ® IoT Developer Kit, IntelÂ® Edison board,
 * cloud platforms, APIs, and other technologies.
 *
 * @hardware Sensors used:\n
 * ULN200XA Stepper Driver x2\n
 * 28BYJ-48 Unipolar Stepper Motor x2\n
 * Analog Joystick
 *
 * @cc
 * @cxx -std=c++1y
 * @ld -lupm-rfr359f -lupm-joystick12 -lupm-uln200xa -lpaho-mqtt3cs -lupm-grove -lboost_system -lboost_thread -lboost_filesystem -lboost_date_time -lpthread -lcurl
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
#include <math.h>

#include <uln200xa.hpp>
#include <joystick12.hpp>

#include "html.h"
#include "css.h"
#include "../lib/crow/crow_all.h"

using namespace std;

// The hardware devices that the example is going to connect to
struct Devices
{
  upm::ULN200XA* stepperOne;
  upm::ULN200XA* stepperTwo;
  upm::Joystick12* joystick;

  Devices(){
  };

  // Initialization function
  void init() {

    // first stepper motor connected to d8, 9, 10, 11
    stepperOne = new upm::ULN200XA(4096, 8, 9, 10, 11);
    stop(stepperOne);

    // second stepper motor connected to 4, 5, 6, 7
    stepperTwo = new upm::ULN200XA(4096, 4, 5, 6, 7);
    stop(stepperTwo);

    // joystick connected to A0 and A1
    joystick = new upm::Joystick12(0,1);
  };

  // Cleanup on exit
  void cleanup() {
    stepperOne->release();
    stepperTwo->release();
    delete stepperOne;
    delete stepperTwo;
    delete joystick;
  }

  //functions to set speed and direction of motors
  //steps 4096 is one full revolution of the motor
  void move(upm::ULN200XA* motor, upm::ULN200XA::ULN200XA_DIRECTION_T direction) {
    motor->setSpeed(5);
    motor->setDirection(direction);
    motor->stepperSteps(512);
  }

  void stop(upm::ULN200XA* motor) {
    motor->setSpeed(5);
    motor->setDirection(upm::ULN200XA::DIR_CW);
    motor->stepperSteps(0);
  }

  void move_1_clockwise() {
    move(stepperOne, upm::ULN200XA::DIR_CW);
  }

  void move_1_counter_clockwise() {
    move(stepperOne, upm::ULN200XA::DIR_CCW);
  }

  void move_2_clockwise() {
    move(stepperTwo, upm::ULN200XA::DIR_CW);
  }

  void move_2_counter_clockwise() {
    move(stepperTwo, upm::ULN200XA::DIR_CCW);
  }

  // function to turn the joystick input -1, 0, or 1
  int scale(float n) {
    auto val = (n - -0.5) / -0.4;
    if (val > 1) { val = 1; }
    if (val < 0) { val = 0; }
    return round(val * 2 - 1);
  }

  // Handle joystick input and movement
  void joystick_move() {
    float x = joystick->getXInput();
    float y = joystick->getYInput();

    if (scale(x) == 1) {
      move_1_clockwise();
    }

    if (scale(x) == -1) {
      move_1_counter_clockwise();
    }

    if (scale(y) == 1) {
      move_2_clockwise();
    }

    if (scale(y) == -1){
      move_2_counter_clockwise();
    }

    usleep(500);
  }
};

// The thread that communicates with devices
void runner(Devices& devices) {
  for(;;){
    devices.joystick_move();
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

  // start worker thread
  std::thread t1(runner, ref(devices));

  // web app
  crow::SimpleApp app;

  CROW_ROUTE(app, "/")
  ([]() {
    std::stringstream text;
    text << index_html;
    return text.str();
  });

  CROW_ROUTE(app, "/one-cw")
    .methods("POST"_method)
  ([]() {
    devices.move_1_clockwise();
    return crow::response("OK");
  });

  CROW_ROUTE(app, "/one-ccw")
    .methods("POST"_method)
  ([]() {
    devices.move_1_counter_clockwise();
    return crow::response("OK");
  });

  CROW_ROUTE(app, "/two-cw")
    .methods("POST"_method)
  ([]() {
    devices.move_2_clockwise();
    return crow::response("OK");
  });

  CROW_ROUTE(app, "/two-ccw")
    .methods("POST"_method)
  ([]() {
    devices.move_2_counter_clockwise();
    return crow::response("OK");
  });

  CROW_ROUTE(app, "/styles.css")
  ([]() {
    std::stringstream text;
    text << styles_css;
    return text.str();
  });

  // start web server
  app.port(4567).multithreaded().run();

  // waits forever on the worker thread
  t1.join();

  return MRAA_SUCCESS;
}
