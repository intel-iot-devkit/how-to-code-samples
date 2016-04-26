/*
* Copyright (c) 2015-2016 Intel Corporation.
*
* Permission is hereby granted, free of charge, to any person ("User") obtaining
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
* User understands, acknowledges, and agrees that: (i) the Software is sample software;
* (ii) the Software is not designed or intended for use in any medical, life-saving
* or life-sustaining systems, transportation systems, nuclear systems, or for any
* other mission-critical application in which the failure of the system could lead to
* critical injury or death; (iii) the Software may not be fully tested and may contain
* bugs or errors; (iv) the Software is not intended or suitable for commercial release;
* (v) no regulatory approvals for the Software have been obtained, and therefore Software
* may not be certified for use in certain countries or environments.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
* LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
* OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
* WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/**
 * @file
 * @ingroup howtocode
 * @brief Equipment activity monitor in C++
 *
 * This equipment-activity application is part of a series of how-to Intel IoT code
 * sample exercises using the Intel® IoT Developer Kit, Intel® Edison board,
 * cloud platforms, APIs, and other technologies.
 *
 * @hardware Sensors used:\n
 * Grove Sound Sensor\n
 * Grove Piezo Vibration Sensor\n
 * Grove RGB LCD\n
 *
 * @cc
 * @cxx -std=c++1y
 * @ld -lupm-i2clcd -lump-mic -lupm-ldt0028 -lpaho-mqtt3cs -lcurl
 *
 * Additional source files required to build this example:
 * @req datastore.cpp
 * @req mqtt.cpp
 *
 * @date 04/04/2016
 */


#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <ctime>
#include <signal.h>

#include <mic.hpp>
#include <ldt0028.hpp>
#include <jhd1313m1.h>

#include "../lib/restclient-cpp/include/restclient-cpp/restclient.h"

#include "datastore.h"
#include "mqtt.h"

const int VIBRATION_THRESHOLD = 500;
const int NOISE_THRESHOLD = 140;

// Call remote datastore server to log activity
void notify(std::string message) {

  std::time_t now = std::time(NULL);
  char mbstr[sizeof "2011-10-08T07:07:09Z"];
  std::strftime(mbstr, sizeof(mbstr), "%FT%TZ", std::localtime(&now));

  std::stringstream text;
  text << "{\"state\":";
  text << "\"" << message << " " << mbstr << "\"}";

  log_mqtt(text.str());
  log_datastore(text.str());
}

// The hardware devices that the example is going to connect to
struct Devices
{
  upm::Microphone *mic = NULL;
  thresholdContext micCtx;
  uint16_t soundBuffer [128];

  upm::LDT0028* vibe;
  upm::Jhd1313m1* screen;

  Devices(){
  };

  // Initialization function
  void init() {
    // mic connected to A0 (analog in)
    mic = new upm::Microphone(0);
    micCtx.averageReading = 0;
    micCtx.runningAverage = 0;
    micCtx.averagedOver   = 2;

    // vibration sensor connected to A2 (analog in)
    vibe = new upm::LDT0028(2);

    // screen connected to the default I2C bus
    screen = new upm::Jhd1313m1(0);
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

Devices devices;

// Exit handler for program
void exit_handler(int param)
{
  devices.cleanup();
  exit(1);
}

// The main function for the example program
int main()
{
  // handles ctrl-c or other orderly exits
  signal(SIGINT, exit_handler);

  // check that we are running on Galileo or Edison
  mraa_platform_t platform = mraa_get_platform_type();
  if ((platform != MRAA_INTEL_GALILEO_GEN1) &&
    (platform != MRAA_INTEL_GALILEO_GEN2) &&
    (platform != MRAA_INTEL_EDISON_FAB_C)) {
    std::cerr << "ERROR: Unsupported platform" << std::endl;
    return MRAA_ERROR_INVALID_PLATFORM;
  }

  // create and initialize UPM devices
  devices.init();
  devices.reset();

  bool movement = false;
  bool noise = false;
  bool inUse = false;

  // every 1 sec, check for movement and noise
  for (;;) {
    movement = devices.is_movement();
    noise = devices.is_noise();
    if ( movement && noise && !inUse ) {
      notify("start");
      devices.message("start");
    } else if ( !(movement && noise) && inUse ) {
      notify("stop");
      devices.message("stop");
    }

    inUse = movement && noise;
    sleep(1);
  }

  return MRAA_SUCCESS;
}
