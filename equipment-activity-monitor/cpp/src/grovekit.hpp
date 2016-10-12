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

const int VIBRATION_THRESHOLD = 500;
const int NOISE_THRESHOLD = 140;

#include <mic.hpp>
#include <ldt0028.hpp>
#include <jhd1313m1.hpp>

using namespace std;

// The hardware devices that the example is going to connect to
struct Devices
{
  upm::Microphone *mic = NULL;
  thresholdContext micCtx;
  uint16_t soundBuffer [128];

  upm::LDT0028* vibe;
  upm::Jhd1313m1* screen;

  int screenBus, micPin, vibePin;

  Devices(){
  };

  // Set pins/init as needed for specific platforms
  void set_pins() {
    mraa_platform_t platform = mraa_get_platform_type();
    switch (platform) {
      case MRAA_INTEL_GALILEO_GEN1:
      case MRAA_INTEL_GALILEO_GEN2:
      case MRAA_INTEL_EDISON_FAB_C:
        screenBus = 0;
        micPin = 0;
        vibePin = 2;
        break;
      case MRAA_GENERIC_FIRMATA:
        screenBus = 0 + 512;
        micPin = 0 + 512;
        vibePin = 2 + 512;
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
          cerr << "ERROR: Base platform " << platform << " on port " << port.c_str() << " for reason " << res << endl;
        }
        screenBus = 0 + 512;
        micPin = 0 + 512;
        vibePin = 2 + 512;
    }
  };

  // Initialization function
  void init() {
    set_pins();

    // mic connected to A0 (analog in)
    mic = new upm::Microphone(micPin);
    micCtx.averageReading = 0;
    micCtx.runningAverage = 0;
    micCtx.averagedOver   = 2;

    // vibration sensor connected to A2 (analog in)
    vibe = new upm::LDT0028(vibePin);

    // screen connected to the default I2C bus
    screen = new upm::Jhd1313m1(screenBus);
  };

  // Cleanup on exit
  void cleanup() {
    delete mic;
    delete vibe;
    delete screen;
  }

  // Reset the display
  void reset() {
    message("ready");
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

  // Is there movement detected by the vibration sensor?
  bool is_movement() {
    return (vibe->getSample() >= VIBRATION_THRESHOLD);
  }

  // Is there noise detected by the sound sensor?
  bool is_noise() {
    int len = mic->getSampledWindow(2, 128, soundBuffer);
    if (len) {
      int thresh = mic->findThreshold(&micCtx, 30, soundBuffer, len);
        if (thresh) {
          return true;
        }
    }

    return false;
  }
};

#endif /* GROVEKIT_HPP_ */
