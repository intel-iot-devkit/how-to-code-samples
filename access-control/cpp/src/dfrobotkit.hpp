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

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sstream>
#include <thread>
#include <ctime>
#include <chrono>
#include <string>

#include <sainsmartks.hpp>
#include <biss0001.hpp>

using namespace std;

// The hardware devices that the example is going to connect to
struct Devices
{
  upm::SAINSMARTKS* screen;
  upm::BISS0001* motion;

  Devices(){
  };

  // Initialization function
  void init() {
    // screen connected to the default I2C bus
    screen = new upm::SAINSMARTKS();

    // motion sensor on digital D4
    motion = new upm::BISS0001(4);
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
