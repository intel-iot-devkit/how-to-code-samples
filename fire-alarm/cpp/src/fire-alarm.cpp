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
 * @brief Fire alarm in C++
 *
 * This fire-alarm application is part of a series of how-to Intel IoT code
 * sample exercises using the Intel® IoT Developer Kit, Intel® Edison board,
 * cloud platforms, APIs, and other technologies.
 *
 * @hardware Sensors used:\n
 * Grove Temperature Sensor\n
 * Grove Buzzer\n
 * Grove RGB LCD\n
 *
 * @cc
 * @cxx -std=c++1y
 * @ld -lupm-i2clcd -lupm-buzzer -lpaho-mqtt3cs -lupm-grove -lssl -lcrypto -lcurl
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
#include <vector>
#include <signal.h>

#include <grove.hpp>
#include <buzzer.hpp>
#include <jhd1313m1.hpp>

#include "../lib/restclient-cpp/include/restclient-cpp/restclient.h"

#include "datastore.h"
#include "mqtt.h"

using namespace std;
#include "../lib/twilio-cplusplus/Utils.h"
#include "../lib/twilio-cplusplus/Rest.h"
#include "../lib/twilio-cplusplus/TwiML.h"
using namespace twilio;

const int ALARM_THRESHOLD = 28;
const string TWILIO_API_VERSION = "2010-04-01";

// Send an SMS notification using Twilio
void send_sms() {
  if (!getenv("TWILIO_SID") || !getenv("TWILIO_TOKEN") ||
      !getenv("TWILIO_TO") || !getenv("TWILIO_FROM")) {
    std::cerr << "Twilio not configured." << std::endl;
    return;
  }

  string response;
  vector<Var> vars;

  Rest t(getenv("TWILIO_SID"), getenv("TWILIO_TOKEN"));

  vars.push_back(Var("To", getenv("TWILIO_TO")));
  vars.push_back(Var("From", getenv("TWILIO_FROM")));
  vars.push_back(Var("Body", "fire alarm!"));
  response = t.request("/" + TWILIO_API_VERSION + "/Accounts/" + getenv("TWILIO_SID") + "/SMS/Messages", "POST", vars);
  cout << response << endl;
}

// Notify remote datastore server to note the alarm status
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
  upm::GroveTemp* temp;
  upm::Buzzer* buzzer;
  upm::Jhd1313m1* screen;

  Devices(){
  };

  // Initialization function
  void init() {
    // touch sensor connected to A0 (analog in)
    temp = new upm::GroveTemp(0);

    // buzzer connected to D5 (digital out)
    buzzer = new upm::Buzzer(5);

    // screen connected to the default I2C bus
    screen = new upm::Jhd1313m1(0);

    stop_alarm();
  };

  // Cleanup on exit
  void cleanup() {
    delete temp;
    delete buzzer;
    delete screen;
  }

  // Reset the fire alarm
  void reset() {
    stop_alarm();
    message("Ready");
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

  // Start the alarm
  void start_alarm() {
    message("Fire alarm!");
    buzzer->playSound(266, 0);
  }

  // Stop the alarm
  void stop_alarm() {
    message("           ");
    buzzer->stopSound();
    buzzer->stopSound();
  }

  // Read the current temperature from the connected sensor
  int temperature() {
    return temp->value();
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

  int currentTemperature = 0;
  int previousTemperature = 0;

  // every 1 second, check the temperature
  for (;;) {
    currentTemperature = devices.temperature();
    std::cerr << "Temp: " << currentTemperature << std::endl;

    if (previousTemperature < ALARM_THRESHOLD &&
        currentTemperature >= ALARM_THRESHOLD) {
      notify();
      devices.start_alarm();
      send_sms();
    }

    if (previousTemperature >= ALARM_THRESHOLD &&
        currentTemperature < ALARM_THRESHOLD) {
      devices.reset();
    }

    previousTemperature = currentTemperature;
    sleep(1);
  }

  return MRAA_SUCCESS;
}
