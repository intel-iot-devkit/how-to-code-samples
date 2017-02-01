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
#include <grovemoisture.hpp>

// The hardware devices that the example is going to connect to
struct Devices
{
  mraa::Gpio* buzzer;
  upm::GroveMoisture* moisture;

  int buzzerPin = 15,
      moisturePin = 2;

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
        buzzerPin += 512;
        moisturePin += 512;
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
        buzzerPin += 512;
        moisturePin += 512;
    }
  }

  // Initialization function
  void init() {
    mraa_init();

    set_pins();

    // buzzer connected to A1 (aka digital out 15)
    buzzer = new mraa::Gpio(buzzerPin);
    buzzer->dir(mraa::DIR_OUT);
    stop_alarm();

    // moisture sensor on analog (A2)
    moisture = new upm::GroveMoisture(moisturePin);
  }

  // Cleanup on exit
  void cleanup() {
    delete buzzer;
    delete moisture;
  }

  // Sounds the alarm
  void alarm() {
    buzzer->write(0);
    sleep(1);
    stop_alarm();
  }

  // Stops the alarm
  void stop_alarm() {
    buzzer->write(0);
  }

  // reads the moisture sensor
  int readMoisture() {
    return moisture->value();
  }
};

#endif /* DFROBOTKIT_HPP_ */
