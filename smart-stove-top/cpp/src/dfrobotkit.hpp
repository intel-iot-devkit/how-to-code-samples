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

// The hardware devices that the example is going to connect to
struct Devices
{
  mraa::Aio* flame;
  mraa::Gpio* buzzer;
  mraa::Aio* temps;

  int flamePin = 2,
      buzzerPin = 15,
      tempsPin = 3;

  float voltageAdj = 1.0;

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
        flamePin += 512;
        buzzerPin += 512;
        tempsPin += 512;
        voltageAdj = 0.66;
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
        flamePin += 512;
        buzzerPin += 512;
        tempsPin += 512;
        voltageAdj = 0.66;
    }
  }

  // Initialization function
  void init() {
    mraa_init();

    set_pins();

    // temperature sensor connected to A3 (analog in)
    temps = new mraa::Aio(tempsPin);

    // buzzer connected to A1 (aka digital out 15)
    buzzer = new mraa::Gpio(buzzerPin);
    buzzer->dir(mraa::DIR_OUT);
    stopAlarm();

    // flame sensor on A2
    flame = new mraa::Aio(flamePin);
  }

  // Cleanup on exit
  void cleanup() {
    delete buzzer;
    delete flame;
    delete temps;
  }

  // Starts the alarm
  void alarm() {
    buzzer->write(1);
  }

  // Stops the alarm
  void stopAlarm() {
    buzzer->write(0);
  }

  // Reads the flame sensor
  bool flameDetected() {
    return (flame->read() >= 800);
  }

  // Reads the ambient temperature sensor
  float objectTemperature(){
	  return ((500 * temps->read()) / 1024) * voltageAdj;
  }
};

#endif /* DFROBOTKIT_HPP_ */
