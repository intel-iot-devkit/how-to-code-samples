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

#include <grovespeaker.hpp>
#include <tp401.hpp>

using namespace std;

#define WARNING_THRESHOLD 200

struct Devices
{
  upm::GroveSpeaker* speaker;
  upm::TP401* sensor;

  int speakerPin, airPin;

  Devices() {
  };

  // Initialization function
  void init() {
    // Set pins/init as needed for specific platforms
    mraa_platform_t platform = mraa_get_platform_type();
    switch (platform) {
      case MRAA_INTEL_GALILEO_GEN1:
      case MRAA_INTEL_GALILEO_GEN2:
      case MRAA_INTEL_EDISON_FAB_C:
        speakerPin = 5;
        airPin = 0;
        break;
      case MRAA_GENERIC_FIRMATA:
        speakerPin = 5 + 512;
        airPin = 0 + 512;
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
        airPin = 0 + 512;
    }

  	// speaker connected to digital D5
    speaker = new upm::GroveSpeaker(speakerPin);

    // air sensor connected to analog A0
    sensor = new upm::TP401(airPin);

    // start sensor warmup process
    warmup();
  };

  // Sounds an audible alarm
  void alarm() {
    speaker->playSound('a', true, "high");
    speaker->playSound('c', true, "high");
    speaker->playSound('g', true, "high");
  }

  // Cleanup on exit
  void cleanup() {
    delete speaker;
    delete sensor;
  }

  // How is the air quality?
  string air_quality(uint16_t value)
  {
    if(value < 30) return "Fresh Air";
    if(value < 100) return "Normal Indoor Air";
    if(value < 200) return "Low Pollution";
    if(value < 300) return "High Pollution - Action Recommended";
    return "Very High Pollution - Take Action Immediately";
  }

  // Warmup the air quality sensor for 3 minutes
  void warmup(){
    cout << sensor->name() << endl;

    fprintf(stdout, "Heating sensor for 3 minutes. Please wait...\n");

    // wait 3 minutes for sensor to warm up
    for(int i = 0; i < 3; i++) {
      if(i) {
        fprintf(stdout, "%d minute(s) passed...\n", i);
      }
      sleep(60);
    }

    fprintf(stdout, "Sensor ready!\n");
  }

  // Check the air qulity by reading the sensor
  uint16_t checkAirQuality(){
    // read sensor value
    uint16_t value = sensor->getSample();

    // read CO ppm (can vary slightly from previous read)
    fprintf(stdout, "Air quality: %4d   %s\n", value, air_quality(value).c_str());

    return value;
  }
};

#endif /* GROVEKIT_HPP_ */
