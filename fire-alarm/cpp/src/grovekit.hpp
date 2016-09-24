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
#include <buzzer.hpp>
#include <jhd1313m1.hpp>

// The hardware devices that the example is going to connect to
struct Devices
{
  upm::GroveTemp* temp;
  upm::Buzzer* buzzer;
  upm::Jhd1313m1* screen;

  Devices(){
  };

  // Initialization function
  void init() {
    // touch sensor connected to A0 (analog in)
    temp = new upm::GroveTemp(0);

    // buzzer connected to D5 (digital out)
    buzzer = new upm::Buzzer(5);

    // screen connected to the default I2C bus
    screen = new upm::Jhd1313m1(0);

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
    std::size_t red   = (color & 0xff0000) >> 16;
    std::size_t green = (color & 0x00ff00) >> 8;
    std::size_t blue  = (color & 0x0000ff);

    std::string text(input);
    text.resize(16, ' ');

    screen->setCursor(0,0);
    screen->write(text);
    screen->setColor(red, green, blue);
  }

  // Start the alarm
  void start_alarm() {
    message("Fire alarm!");
    buzzer->playSound(266, 0);
  }

  // Stop the alarm
  void stop_alarm() {
    message("           ");
    buzzer->stopSound();
    buzzer->stopSound();
  }

  // Read the current temperature from the connected sensor
  int temperature() {
    return temp->value();
  }
};

#endif /* GROVEKIT_HPP_ */
