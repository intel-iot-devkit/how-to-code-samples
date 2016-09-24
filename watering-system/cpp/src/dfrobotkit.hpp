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

#ifndef DFROBOTKIT_HPP_
#define DFROBOTKIT_HPP_

#include <mraa.hpp>
#include <grovemoisture.hpp>

using namespace std;

// The hardware devices that the example is going to connect to
struct Devices
{
  upm::GroveMoisture* moisture;
  mraa::Gpio* pump;

  int moistureReading = 0;
  bool turnedOn = false;
  bool turnedOff = false;

  Devices() {
  };

  // Initialization function
  void init() {
    // pump attached to A2 (aka D16)
    pump = new mraa::Gpio(16);
    pump->dir(mraa::DIR_OUT);
    pump->write(0);

    // moisture sensor attached to A1
    moisture = new upm::GroveMoisture(1);
  };

  // Cleanup on exit
  void cleanup() {
    delete pump;
    delete moisture;
  }

  // Reads the current value from the moisture sensor
  int readMoisture() {
    return moisture->value();
  }

  // Reads the current value from the flow sensor
  int readFlow() {
    if (turnedOn)
      return 1;
    if (turnedOff)
      return 0;
    return 0;
  }

  // Is the water supposed to be turned on?
  bool turned_on() {
    return turnedOn;
  }

  // Is the water supposed to be turned off?
  bool turned_off() {
    return turnedOff;
  }

  // Turn on the water
  void turn_on() {
    if (turnedOn) return;
    pump->write(1);
    turnedOn = true;
    turnedOff = false;
    message("on");
  }

  // Turn off the water
  void turn_off() {
    if (turnedOff) return;
    pump->write(0);
    turnedOn = false;
    turnedOff = true;
    message("off");
  }

  // Displays message to console
  void message(string msg){
    cout << msg << endl;
  }
};

#endif /* DFROBOTKIT_HPP_ */
