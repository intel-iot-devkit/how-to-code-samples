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
  mraa::Aio* accelX;
  mraa::Aio* accelY;
  mraa::Aio* accelZ;
  upm::SAINSMARTKS* screen;

  Devices(){
  };

  // Initialization function
  void init() {
    // accelerometer connected to A1-A3
    accelX = new mraa::Aio(3);
    accelY = new mraa::Aio(2);
    accelZ = new mraa::Aio(1);

    // screen connected to the default pins
    screen = new upm::SAINSMARTKS();
  };

  // Cleanup on exit
  void cleanup() {
    delete accelX;
    delete accelY;
    delete accelZ;
    delete screen;
  }

  // Reset the display
  void reset() {
    message("quakebot ready");
  }

  // Display message when checking for earthquake
  void checking() {
    message("checking...");
  }

  // Display message when there really has been a recent earthquake
  void warning() {
    message("Earthquake!");
  }

  // Display message when a false alarm
  void noquake() {
    message("No quake.");
  }

  // Display a message on the LCD
  void message(const std::string& input, const std::size_t color = 0x0000ff) {
	  cerr << input << endl;
    std::string text(input);
    text.resize(16, ' ');

    screen->setCursor(0,0);
    screen->write(text);
  }

  // Read the current data from the accelerometer
  bool getAcceleration() {
    int ax, ay, az;
    ax = accelX->read();
    ay = accelY->read();
    az = accelZ->read();
    return (ax > 600 || ay > 600 || az > 600);
  }
};

#endif /* DFROBOTKIT_HPP_ */
