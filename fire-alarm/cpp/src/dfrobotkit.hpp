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
#include <sainsmartks.hpp>

using namespace std;

// The hardware devices that the example is going to connect to
struct Devices
{
  mraa::Aio* temp;
  mraa::Gpio* buzzer;
  upm::SAINSMARTKS* screen;

  Devices(){
  };

  // Initialization function
  void init() {
    mraa_init();

    // temperature sensor connected to A1 (analog in)
    temp = new mraa::Aio(1);

    // buzzer connected to A2 (aka digital out 16)
    buzzer = new mraa::Gpio(16);
    buzzer->dir(mraa::DIR_OUT);

    // screen connected to the default pins
    screen = new upm::SAINSMARTKS();

    stop_alarm();
  };

  // Cleanup on exit
  void cleanup() {
    delete temp;
    delete buzzer;
    delete screen;
  }

  // Reset the fire alarm
  void reset() {
    stop_alarm();
    message("Ready");
  }

  // Display a message on the LCD
  void message(const std::string& input, const std::size_t color = 0x0000ff) {
    cerr << input << endl;
    std::string text(input);
    text.resize(16, ' ');

    screen->setCursor(0,0);
    screen->write(text);
  }

  // Start the alarm
  void start_alarm() {
    message("Fire alarm!");
    buzzer->write(1);
  }

  // Stop the alarm
  void stop_alarm() {
    message("           ");
    buzzer->write(0);
  }

  // Read the current temperature from the connected sensor
  int temperature() {
    return (500 * temp->read()) / 1024;
  }
};

#endif /* DFROBOTKIT_HPP_ */
