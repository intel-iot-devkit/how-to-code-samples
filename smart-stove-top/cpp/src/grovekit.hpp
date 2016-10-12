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
#include <yg1006.hpp>
#include <otp538u.hpp>
#include <grovespeaker.hpp>

using namespace std;

// The hardware devices that the example is going to connect to
struct Devices
{
  upm::GroveSpeaker* speaker;
  upm::YG1006* flame;
  upm::OTP538U* temps;

  int speakerPin, flamePin, tempPin1, tempPin2;
  float referenceVoltage;

  Devices(){
  };

  // Set pins/init as needed for specific platforms
  void set_pins() {
    mraa_platform_t platform = mraa_get_platform_type();
    switch (platform) {
      case MRAA_INTEL_GALILEO_GEN1:
      case MRAA_INTEL_GALILEO_GEN2:
      case MRAA_INTEL_EDISON_FAB_C:
        speakerPin = 5;
        flamePin = 4;
        tempPin1 = 0;
        tempPin2 = 1;
        referenceVoltage = 5.0;
        break;
      case MRAA_GENERIC_FIRMATA:
        speakerPin = 5 + 512;
        flamePin = 4 + 512;
        tempPin1 = 0 + 512;
        tempPin2 = 1 + 512;
        referenceVoltage = 3.3;
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
        speakerPin = 5 + 512;
        flamePin = 4 + 512;
        tempPin1 = 0 + 512;
        tempPin2 = 1 + 512;
        referenceVoltage = 3.3;
    }
  }

  // Initialization function
  void init() {
    set_pins();

    // speaker connected to D5 (digital out)
    speaker = new upm::GroveSpeaker(speakerPin);

    // flame sensor on D4
    flame = new upm::YG1006(flamePin);

    // Instantiate a OTP538U on analog pins A0 and A1
    // A0 is used for the Ambient Temperature and A1 is used for the
    // Object temperature.
    // only plug ir temp sensor into A0 with this code
    temps = new upm::OTP538U(tempPin1, tempPin2, referenceVoltage);
  }

  // Cleanup on exit
  void cleanup() {
    delete speaker;
    delete flame;
    delete temps;
  }

  // Starts the alarm
  void alarm() {
    speaker->playSound('c', true, "high");
  }

  // Reads the flame sensor
  bool flameDetected() {
    return flame->flameDetected();
  }

  // Reads the IR temperature sensor
  float objectTemperature(){
    return temps->objectTemperature();
  }
};

#endif /* GROVEKIT_HPP_ */
