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

// The hardware devices that the example is going to connect to
struct Devices
{
  mraa::Aio* flame;
  mraa::Gpio* buzzer;
  mraa::Aio* temps;

  Devices(){
  };

  // Initialization function
  void init() {
    mraa_init();

    // temperature sensor connected to A1 (analog in)
    temps = new mraa::Aio(1);

    // buzzer connected to A2 (aka digital out 16)
    buzzer = new mraa::Gpio(16);
    buzzer->dir(mraa::DIR_OUT);
    stopAlarm();

    // flame sensor on A3
    flame = new mraa::Aio(3);
  }

  // Cleanup on exit
  void cleanup() {
    delete buzzer;
    delete flame;
    delete temps;
  }

  // Starts the alarm
  void alarm() {
    buzzer->write(1);
  }

  // Stops the alarm
  void stopAlarm() {
    buzzer->write(0);
  }

  // Reads the flame sensor
  bool flameDetected() {
    return (flame->read() >= 800);
  }

  // Reads the ambient temperature sensor
  float objectTemperature(){
	   return (500 * temps->read()) / 1024;
  }
};

#endif /* DFROBOTKIT_HPP_ */
