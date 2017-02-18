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

#include <mraa.hpp>
#include <mic.hpp>
#include <sainsmartks.hpp>

using namespace std;

// The hardware devices that the example is going to connect to
struct Devices
{
  upm::Microphone *mic = NULL;
  thresholdContext micCtx;
  uint16_t soundBuffer [128];

  upm::SAINSMARTKS* screen;

  int micPin = 3,
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
        micPin += 512;
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
        micPin += 512;
    }
  }

  // Initialization function
  void init() {
    mraa_init();

    set_pins();

    // mic connected to A3 (analog in)
    mic = new upm::Microphone(micPin);
    micCtx.averageReading = 0;
    micCtx.runningAverage = 0;
    micCtx.averagedOver   = 2;

    // screen connected to the shield itself
    screen = new upm::SAINSMARTKS(screen1, screen2, screen3, screen4, screen5, screen6, screenPad);
  };

  // Cleanup on exit
  void cleanup() {
    delete mic;
    delete screen;
  }

  // Reset the display
  void reset() {
    message("ready");
  }

  // Display a message on the LCD
  void message(const std::string& input, const std::size_t color = 0x0000ff) {
    cerr << input << endl;
    std::string text(input);
    text.resize(16, ' ');

    screen->setCursor(0,0);
    screen->write(text);
  }

  // get mic level
  int get_mic_level() {
    int len = mic->getSampledWindow(2, 128, soundBuffer);
    if (len) {
      int thresh = mic->findThreshold(&micCtx, 30, soundBuffer, len);
      return thresh / 100;
    }

    return 0;
  }
};

#endif /* DFROBOTKIT_HPP_ */
