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
 * @brief Robot arm in C++
 *
 * This robot-arm application is part of a series of how-to Intel IoT code
 * sample exercises using the Intel® IoT Developer Kit, Intel® Edison board,
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

  int joystickPin1 = 0,
      joystickPin2 = 1,
      stepLeftInputPin1 = 9,
      stepLeftInputPin2 = 10,
      stepLeftInputPin3 = 11,
      stepLeftInputPin4 = 12,
      stepRightInputPin1 = 4,
      stepRightInputPin2 = 5,
      stepRightInputPin3 = 6,
      stepRightInputPin4 = 7;

  float voltageAdjust = 1.0;

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
         joystickPin1 += 512;
         joystickPin2 += 512;
         stepLeftInputPin1 += 512;
         stepLeftInputPin2 += 512;
         stepLeftInputPin3 += 512;
         stepLeftInputPin4 += 512;
         stepRightInputPin1 += 512;
         stepRightInputPin2 += 512;
         stepRightInputPin3 += 512;
         stepRightInputPin4 += 512;
         voltageAdjust = 1.33;

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
        joystickPin1 += 512;
        joystickPin2 += 512;
        stepLeftInputPin1 += 512;
        stepLeftInputPin2 += 512;
        stepLeftInputPin3 += 512;
        stepLeftInputPin4 += 512;
        stepRightInputPin1 += 512;
        stepRightInputPin2 += 512;
        stepRightInputPin3 += 512;
        stepRightInputPin4 += 512;
        voltageAdjust = 1.33;
    }
  }

  // Initialization function
  void init() {
    set_pins();

    // first stepper motor connected to d8, 9, 10, 11
    stepperOne = new upm::ULN200XA(4096, stepLeftInputPin1, stepLeftInputPin2, stepLeftInputPin3, stepLeftInputPin4);
    stop(stepperOne);

    // second stepper motor connected to 4, 5, 6, 7
    stepperTwo = new upm::ULN200XA(4096, stepRightInputPin1, stepRightInputPin2, stepRightInputPin3, stepRightInputPin4);
    stop(stepperTwo);

    // joystick connected to A0 and A1
    joystick = new upm::Joystick12(joystickPin1, joystickPin2);
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
  void move(upm::ULN200XA* motor, ULN200XA_DIRECTION_T direction) {
    motor->setSpeed(5);
    motor->setDirection(direction);
    motor->stepperSteps(512);
  }

  void stop(upm::ULN200XA* motor) {
    motor->setSpeed(5);
    motor->setDirection(ULN200XA_DIR_CW);
    motor->stepperSteps(0);
  }

  void move_1_clockwise() {
    move(stepperOne, ULN200XA_DIR_CW);
  }

  void move_1_counter_clockwise() {
    move(stepperOne, ULN200XA_DIR_CCW);
  }

  void move_2_clockwise() {
    move(stepperTwo, ULN200XA_DIR_CW);
  }

  void move_2_counter_clockwise() {
    move(stepperTwo, ULN200XA_DIR_CCW);
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
    float x = joystick->getXInput() * voltageAdjust;
    float y = joystick->getYInput() * voltageAdjust;

    if (scale(x) == 1) {
      std::cout << "Move 1 clockwise." << std::endl;
      move_1_clockwise();
    }

    if (scale(x) == -1) {
      std::cout << "Move 1 counter-clockwise." << std::endl;
      move_1_counter_clockwise();
    }

    if (scale(y) == 1) {
      std::cout << "Move 2 clockwise." << std::endl;
      move_2_clockwise();
    }

    if (scale(y) == -1){
      std::cout << "Move 2 counter-clockwise." << std::endl;
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
