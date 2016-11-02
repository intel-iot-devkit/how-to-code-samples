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

/**
 * @file
 * @ingroup howtocode
 * @brief Access control in C++
 *
 * This access-control application is part of a series of how-to Intel IoT code
 * sample exercises using the Intel® IoT Developer Kit, Intel® Edison board,
 * cloud platforms, APIs, and other technologies.
 *
 * @hardware Sensors used:\n
 * Grove LCD RGB Backlight\n
 * Grove PIR Motion Sensor\n
 *
 * @cc
 * @cxx -std=c++1y
 * @ld -lupm-i2clcd -lupm-biss0001 -lpaho-mqtt3cs -lupm-grove -lboost_system -lboost_thread -lboost_filesystem -lboost_date_time -lpthread -lcurl
 *
 * Additional source files required to build this example:
 * @req datastore.cpp
 * @req mqtt.cpp
 *
 * @date 04/04/2016
 */

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sstream>
#include <thread>
#include <ctime>
#include <chrono>
#include <string>

#include "../lib/crow/crow_all.h"
#include "../src/html.h"
#include "../src/css.h"

#include "kits.h"
#if INTEL_IOT_KIT == DFROBOTKIT
#include "dfrobotkit.hpp"
#else
#include "grovekit.hpp"
#endif

#include "../lib/restclient-cpp/include/restclient-cpp/restclient.h"

#include "datastore.h"
#include "mqtt.h"

using namespace std;

Devices devices;

bool countdownStarted = false;
bool disarmed = false;
bool alarmTriggered = false;

// The time that the motion was detected
chrono::time_point<chrono::system_clock> detectTime;

// The time that the alarm was disarmed
chrono::time_point<chrono::system_clock> disarmTime;

// Notify the remote datastore
void notify(std::string message) {
  time_t now = std::time(NULL);
  char mbstr[sizeof "2011-10-08T07:07:09Z"];
  strftime(mbstr, sizeof(mbstr), "%FT%TZ", localtime(&now));

  stringstream text;
  text << "{\"state\":";
  text << "\"" << message << " " << mbstr << "\"}";

  log_mqtt(text.str());
  log_datastore(text.str());
}

// Starts a countdown to sounding alarm after a person is detected
void start_alarm_countdown() {
  countdownStarted = true;
  detectTime = chrono::system_clock::now();
  string msg = "Person detected";
  devices.message(msg, 0xff00ff);
  notify(msg);
}

// Triggers alarm after a person has been detected, but did not enter the access code
void trigger_alarm() {
  alarmTriggered = true;
  string msg = "Alarm triggered!";
  devices.message(msg, 0xff00ff);
  notify(msg);
}

// Disarms the access control system
void disarm() {
  disarmTime = chrono::system_clock::now();
  disarmed = true;
  countdownStarted = false;
  alarmTriggered = false;
}

// Resets the access control system
void reset() {
  disarmed = false;
  countdownStarted = false;
  alarmTriggered = false;
}

// Helper function to determin how long since a time point something happened
int elapsed_since(chrono::time_point<chrono::system_clock> tp) {
  chrono::duration<double> elapsed;
  chrono::time_point<chrono::system_clock> now;
  now = chrono::system_clock::now();
  elapsed = now - tp;
  return elapsed.count();
}

// Handles all of the detection logic for the access control system
void detect() {
  if (alarmTriggered) {
    if (elapsed_since(detectTime) > 120) reset();
  } else if (disarmed) {
    if (elapsed_since(disarmTime) > 120) reset();
  } else if (countdownStarted) {
    if (elapsed_since(detectTime) > 30) trigger_alarm();
  } else if (devices.motion->value()) {
    start_alarm_countdown();
  } else {
    devices.message("Monitoring...");
  }
}


// The access code to be entered to disarm alarm system
string access_code() {
  if (!getenv("CODE")) {
    return "4321";
  } else {
    return getenv("CODE");
  }
}

// Function called by worker thread for device communication
void runner(Devices& devices) {
  for (;;) {
    detect();
    usleep(500);
  }
}

// Exit handler for program
void exit_handler(int param)
{
  devices.cleanup();
  exit(1);
}

// The main function for the example program
int main() {
  // Handles ctrl-c or other orderly exits
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

  // start worker thread for device communication
  std::thread t1(runner, std::ref(devices));

  // define web server & routes
  crow::SimpleApp app;

  CROW_ROUTE(app, "/")
  ([]() {
    std::stringstream text;
    text << index_html;
    return text.str();
  });

  CROW_ROUTE(app, "/alarm")
  ([](const crow::request& req) {
    if(req.url_params.get("code") != nullptr) {
      if (access_code() == req.url_params.get("code")) {
        disarm();
      } else {
        notify("invalid code");
      }
    }

    return crow::response("OK");
  });

  CROW_ROUTE(app, "/styles.css")
  ([]() {
    std::stringstream text;
    text << styles_css;
    return text.str();
  });

  // start web server
  app.port(3000).multithreaded().run();

  // wait forever for the thread to exit
  t1.join();

  return MRAA_SUCCESS;
}
