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

#include <math.h>

// The hardware devices that the example is going to connect to
struct Devices
{
  upm::GroveRotary* rotary;
  upm::GroveButton* button;
  upm::Buzzer* buzzer;
  upm::Jhd1313m1* screen;

  Devices(){
  };

  // Initialization function
  void init() {
    // rotary connected to A0 (analog in)
    rotary = new upm::GroveRotary(0);

    // button connected to D4 (digital out)
    button = new upm::GroveButton(4);

    // buzzer connected to D5 (digital out)
    buzzer = new upm::Buzzer(5);
    stop_buzzing();

    // screen connected to the default I2C bus
    screen = new upm::Jhd1313m1(0);
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
    std::size_t newColor;
    newColor = round((rotary->abs_value() / 1020) * 255);
    if (newColor > 255) newColor = 255;
    if (newColor < 0) newColor = 0;

    screen->setColor(newColor, newColor, newColor);
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

  // Starts the buzzer making noise
  void start_buzzing() {
    buzzer->setVolume(0.5);
    buzzer->playSound(2600, 0);
  }

  // Stops the buzzer making noise
  void stop_buzzing() {
  buzzer->setVolume(0);
    buzzer->stopSound();
    buzzer->stopSound();
  }
};

#endif /* GROVEKIT_HPP_ */
