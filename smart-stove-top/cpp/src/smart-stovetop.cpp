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
 * @brief Smart stove top in C++
 *
 * This smart-stovetop application is part of a series of how-to Intel IoT code
 * sample exercises using the Intel® IoT Developer Kit, Intel® Edison board,
 * cloud platforms, APIs, and other technologies.
 *
 * @hardware Sensors used:\n
 * Grove Speaker\n
 * Grove Flame Sensor\n
 * Grove IR Temerature Sensor\n
 *
 * @cc
 * @cxx -std=c++1y
 * @ld -lupm-grovespeaker -lupm-yg1006 -lupm-otp538u -lpaho-mqtt3cs -lupm-grove -lboost_system -lboost_thread -lboost_filesystem -lboost_date_time -lpthread -lcurl
 *
 * Additional source files required to build this example:
 * @req datastore.cpp
 * @req mqtt.cpp
 *
 * @date 04/04/2016
 */
#include <iostream>
#include <string>
#include <signal.h>

#include <grove.hpp>
#include <yg1006.hpp>
#include <otp538u.hpp>
#include <grovespeaker.hpp>

#include "../lib/restclient-cpp/include/restclient-cpp/restclient.h"
#include "datastore.h"
#include "mqtt.h"
#include "../lib/crow/crow_all.h"
#include "html.h"
#include "styles.h"

#define OTP538U_AREF 5.0

using namespace std;

// Global temp value for app; will change when you use app
float myTemp = 1000;

// Send notification to remote datastore
void notify() {

	  std::time_t now = std::time(NULL);
	  char mbstr[sizeof "2011-10-08T07:07:09Z"];
	  std::strftime(mbstr, sizeof(mbstr), "%FT%TZ", std::localtime(&now));

	  std::stringstream text;
	  text << "{\"value\":";
	  text << "\"" << mbstr << "\"}";

	  log_mqtt(text.str());
	  log_datastore(text.str());
}

// The hardware devices that the example is going to connect to
struct Devices
{
  upm::GroveSpeaker* speaker;
  upm::YG1006* flame;
  upm::OTP538U* temps;

  Devices(){
  };

  // Initialization function
  void init() {
    // speaker connected to D5 (digital out)
    speaker = new upm::GroveSpeaker(5);

    // flame sensor on D4
    flame = new upm::YG1006(4);

    // Instantiate a OTP538U on analog pins A0 and A1
    // A0 is used for the Ambient Temperature and A1 is used for the
    // Object temperature.
    // only plug ir temp sensor into A0 with this code
    temps = new upm::OTP538U(0, 1, OTP538U_AREF);
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

  // Reads the flame sensor every 1 second
  void senseFlame() {
    for(;;) {
      bool val = flame->flameDetected();
      if (val) {
        cout << "Flame detected." << endl;
        alarm();
      }

      else
      {
        cout << "No flame detected." << endl;
        sleep(1);
      }
    }
  }

  // Continously reads the IR temperature sensor every 1 minute for
  // both object and ambient temperature, and compares the two.
  // Sounds alarm if object temp is higher than set temp.
  void senseTemp(){
    for(;;) {
      cout << "Ambient temp: " << std::fixed << setprecision(2)
           << temps->ambientTemperature()
           << " C, Object temp: " << temps->objectTemperature()
           << " C" << endl;

      float temp1 = temps->objectTemperature();

      if(temp1 >= myTemp){
        alarm();
        notify();
      }
      else
        sleep(60);
    }
  }
};

// Keeps the flame sensor running
void runFlame(Devices& devices) {
  devices.senseFlame();
}

// Keeps the ir temperature sensor running
void runTemp(Devices& devices) {
  devices.senseTemp();
}

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
    cerr << "ERROR: Unsupported platform" << endl;
    return MRAA_ERROR_INVALID_PLATFORM;
  }

  // create and initialize UPM devices
  devices.init();

  // start worker threads for device communication
  thread t1(runTemp, ref(devices));
  thread t2(runFlame, ref(devices));

  // define web server & routes
  crow::SimpleApp app;

  CROW_ROUTE(app, "/")
    ([](const crow::request& req) {

      if(req.url_params.get("temp") != nullptr) {
        double newTemp = boost::lexical_cast<double>(req.url_params.get("temp"));
        cout << "The value of 'temp' is " <<  newTemp << endl;
        myTemp = newTemp;
      }

      stringstream text;
      text << index_html;
      return text.str();
  });

  CROW_ROUTE(app, "/styles.css")
  ([]() {
    std::stringstream text;
    text << styles_css;
    return text.str();
  });

  // start web server
  app.port(3000).multithreaded().run();

  // waits forever on worker threads
  t1.join();
  t2.join();

  return 0;
}
