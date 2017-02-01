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

#ifndef DFROBOTKIT_HPP_
#define DFROBOTKIT_HPP_

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sstream>
#include <thread>
#include <ctime>
#include <chrono>
#include <string>

#include <mraa.hpp>
#include <sainsmartks.hpp>
#include <biss0001.hpp>

using namespace std;

// The hardware devices that the example is going to connect to
struct Devices
{
  upm::SAINSMARTKS* screen;
  upm::BISS0001* motion;

  int motionPin = 16,
      screen1 = 8,
      screen2 = 9,
      screen3 = 4,
      screen4 = 5,
      screen5 = 6,
      screen6 = 7,
      screenPad = 0;

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
        screen1 += 512;
        screen2 += 512;
        screen3 += 512;
        screen4 += 512;
        screen5 += 512;
        screen6 += 512;
        screenPad += 512;
        motionPin += 512;
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
        screen1 += 512;
        screen2 += 512;
        screen3 += 512;
        screen4 += 512;
        screen5 += 512;
        screen6 += 512;
        screenPad += 512;
        motionPin += 512;
    }
  }

  // Initialization function
  void init() {
    mraa_init();

    set_pins();

    // screen connected to the shield itself
    screen = new upm::SAINSMARTKS(screen1, screen2, screen3, screen4, screen5, screen6, screenPad);

    // motion sensor on pin A2, acting as digital D16
    motion = new upm::BISS0001(motionPin);
  };

  // Cleanup on exit
  void cleanup() {
    delete screen;
    delete motion;
  }

  // Display a message on the LCD
  void message(const string& input, const size_t color = 0x0000ff) {
    cout << input << std::endl;

    string text(input);
    text.resize(16, ' ');

    screen->setCursor(0,0);
    screen->write(text);
  }
};

#endif /* DFROBOTKIT_HPP_ */
