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
 * @brief Doorbell in C++
 *
 * This smart doorbell application is part of a series of how-to Intel IoT code
 * sample exercises using the Intel® IoT Developer Kit, Intel® Edison board,
 * cloud platforms, APIs, and other technologies.
 *
 * @hardware Sensors used:\n
 * Grove Touch Sensor\n
 * Grove Buzzer\n
 * Grove LCD RGB Backlight\n
 *
 * @cc
 * @cxx -std=c++1y
 * @ld -lupm-ttp223 -lupm-buzzer -lupm-i2clcd -lupm-jhd1313m1 -lcurl
 *
 * Additional source files required to build this example:
 * @req datastore.cpp
 * @req mqtt.cpp
 *
 * @date 09/09/2016
 */

#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <signal.h>

using namespace std;

#include "kits.h"
#if INTEL_IOT_KIT == DFROBOTKIT
#include "dfrobotkit.hpp"
#else
#include "grovekit.hpp"
#endif

#include "datastore.h"
#include "mqtt.h"

Devices devices;

// Call datastore/mqtt to increment the count of visitors who rang the doorbell
void increment() {
  increment_datastore();
  increment_mqtt();
}

// Visual and audible notification that someone is at the door
void ring() {
  increment();
  devices.ring();
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
  // Handles ctrl-c or other orderly exits
  signal(SIGINT, exit_handler);

  // create and initialize UPM devices
  devices.init();
  devices.reset();

  bool wasPressed = false;
  bool currentlyPressed = false;

  cerr << "Doorbot ready." << endl;

  for (;;) {
    currentlyPressed = devices.is_pressed();
    if ( currentlyPressed && ! wasPressed ) {
      ring();
    } else if (! currentlyPressed && wasPressed ) {
      devices.reset();
    }

    wasPressed = currentlyPressed;
    sleep(1);
  }

  return MRAA_SUCCESS;
}
