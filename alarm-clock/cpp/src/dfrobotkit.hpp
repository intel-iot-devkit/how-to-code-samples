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
#include <grove.hpp>
#include <sainsmartks.hpp>

#include <math.h>

// The DFRobot hardware devices that the example is going to connect to
struct Devices
{
  upm::GroveRotary* rotary;
  upm::GroveButton* button;
  mraa::Gpio* buzzer;
  upm::SAINSMARTKS* screen;

  Devices(){
  };

  // Initialization function
  void init() {
    mraa_init();
    
    // rotary connected to A3 (analog in)
    rotary = new upm::GroveRotary(3);

    // button connected to A2 (aka digital out 16)
    button = new upm::GroveButton(16);

    // buzzer connected to A1 (aka digital out 15)
    buzzer = new mraa::Gpio(15);
    buzzer->dir(mraa::DIR_OUT);
    stop_buzzing();

    // screen connected to the default pins
    upm::SAINSMARTKS* screen = new upm::SAINSMARTKS();
  };

  // Cleanup on exit
  void cleanup() {
    delete rotary;
    delete button;
    delete buzzer;
    delete screen;
  }

  // Display the current time on the LCD
  void display_time() {
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, 80, "%I:%M:%S", timeinfo);
    std::string str(buffer);

    message(str, 0x00ff00);
  }

  // Adjust the brightness of the backlight on the LCD
  void adjust_brightness()
  {
    std::size_t newLevel;
    newLevel = round((rotary->abs_value() / 1020) * 255);
    if (newLevel > 255) {
    	screen->backlightOn();
    } else {
    	screen->backlightOff();
    }
  }

  // Display a message on the LCD
  void message(const std::string& input, const std::size_t color = 0x0000ff) {
    std::string text(input);
    text.resize(16, ' ');

    screen->setCursor(0,0);
    screen->write(text);
  }

  // Starts the buzzer making noise
  void start_buzzing() {
    buzzer->write(1);
  }

  // Stops the buzzer making noise
  void stop_buzzing() {
	  buzzer->write(0);
  }
};

#endif /* DFROBOTKIT_HPP_ */
