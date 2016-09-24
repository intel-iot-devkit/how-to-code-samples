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

#ifndef GROVEKIT_HPP_
#define GROVEKIT_HPP_

#include <grove.hpp>
#include <yg1006.hpp>
#include <otp538u.hpp>
#include <grovespeaker.hpp>

// The hardware devices that the example is going to connect to
struct Devices
{
  upm::GroveSpeaker* speaker;
  upm::YG1006* flame;
  upm::OTP538U* temps;

  Devices(){
  };

  // Initialization function
  void init() {
    // speaker connected to D5 (digital out)
    speaker = new upm::GroveSpeaker(5);

    // flame sensor on D4
    flame = new upm::YG1006(4);

    // Instantiate a OTP538U on analog pins A0 and A1
    // A0 is used for the Ambient Temperature and A1 is used for the
    // Object temperature.
    // only plug ir temp sensor into A0 with this code
    temps = new upm::OTP538U(0, 1, OTP538U_AREF);
  }

  // Cleanup on exit
  void cleanup() {
    delete speaker;
    delete flame;
    delete temps;
  }

  // Starts the alarm
  void alarm() {
    speaker->playSound('c', true, "high");
  }

  // Reads the flame sensor
  bool flameDetected() {
    return flame->flameDetected();
  }

  // Reads the IR temperature sensor
  float objectTemperature(){
    return temps->objectTemperature();
  }
};

#endif /* GROVEKIT_HPP_ */
