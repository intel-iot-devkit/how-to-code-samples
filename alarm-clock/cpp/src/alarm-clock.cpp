/*
* Copyright (c) 2015-2016 Intel Corporation.
*
* Permission is hereby granted, free of charge, to any person (“User”) obtaining
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
 * @brief Alarm clock in C++
 *
 * This alarm-clock application is part of a series of how-to Intel IoT code
 * sample exercises using the IntelÂ® IoT Developer Kit, IntelÂ® Edison board,
 * cloud platforms, APIs, and other technologies.
 *
 * @hardware Sensors used:\n
 * Grove Rotary Angle Sensor\n
 * Grove Button\n
 * Grove Buzzer\n
 * Grove RGB LCD\n
 *
 * @cc
 * @cxx -std=c++1y
 * @ld -lupm-i2clcd -lupm-buzzer -lpaho-mqtt3cs -lupm-grove -lboost_system -lboost_thread -lboost_filesystem -lboost_date_time -lpthread -lcurl
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

#include <grove.h>
#include <buzzer.h>
#include <jhd1313m1.h>

#include "../lib/restclient-cpp/include/restclient-cpp/restclient.h"

#include "datastore.h"
#include "mqtt.h"

#include "../lib/crow/crow_all.h"
#include "index_html.h"
#include "styles_css.h"

// The hardware devices that the example is going to connect to
struct Devices
{
  upm::GroveRotary* rotary;
  upm::GroveButton* button;
  upm::Buzzer* buzzer;
  upm::Jhd1313m1* screen;

  Devices(){
  };

  // Initialization function
  void init() {
    // rotary connected to A0 (analog in)
    rotary = new upm::GroveRotary(0);

    // button connected to D4 (digital out)
    button = new upm::GroveButton(4);

    // buzzer connected to D5 (digital out)
    buzzer = new upm::Buzzer(5);
    stop_buzzing();

    // screen connected to the default I2C bus
    screen = new upm::Jhd1313m1(0);
  };

  // Cleanup on exit
  void cleanup() {
    delete rotary;
    delete button;
    delete buzzer;
    delete screen;
  }

  // Display the current time on the LCD
  void display_time() {
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, 80, "%I:%M:%S", timeinfo);
    std::string str(buffer);

    message(str, 0x00ff00);
  }

  // Adjust the brightness of the backlight on the LCD
  void adjust_brightness()
  {
    std::size_t newColor;
    newColor = std::round((rotary->abs_value() / 1020) * 255);
    if (newColor > 255) newColor = 255;
    if (newColor < 0) newColor = 0;

    screen->setColor(newColor, newColor, newColor);
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

  // Starts the buzzer making noise
  void start_buzzing() {
    buzzer->setVolume(0.5);
    buzzer->playSound(2600, 0);
  }

  // Stops the buzzer making noise
  void stop_buzzing() {
  buzzer->setVolume(0);
    buzzer->stopSound();
    buzzer->stopSound();
  }
};

// Is the alarm currently ringing?
bool alarmRinging = false;

// The time that the alarm is currently set for
std::time_t alarmTime ;

// Useful function used for checking how much time is left before alarm
double countdown(std::time_t& target) {
  time_t rawtime;
  struct tm* timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  return std::difftime(mktime(timeinfo), target);
}

// How much time from when the alarm went off, to when user pushed button
double elapsed(std::time_t& target) {
  return countdown(target);
}

// Is it time to get up now?
bool time_for_alarm(std::time_t& alarm) {
  double remaining = countdown(alarm);

  if (remaining > 0 && remaining < 5 && !alarmRinging) {
    return true;
  } else return false;
}

// Call datastore/mqtt server to log how long it took to wake up today
void log_wakeup() {
  double duration = elapsed(alarmTime);
  std::cerr << "Alarm duration: " << std::to_string(duration) << std::endl;

  std::stringstream text;
  text << "{\"value\": \"" << std::to_string(duration) << "\"}";

  log_mqtt(text.str());
  log_datastore(text.str());
}

// Call weather underground API to get current weather conditions
std::string get_weather() {
  if (!getenv("API_KEY")) {
    std::cerr << "Weather Underground API_KEY not configured." << std::endl;
    return "";
  }

  std::string location = getenv("LOCATION") ? getenv("LOCATION") : "CA/San_Francisco";
  std::stringstream url;
  url << "http://api.wunderground.com/api/" << getenv("API_KEY") << "/conditions/q/" << location << ".json";

  RestClient::headermap headers;
  headers["Accept"] = "application/json";

  RestClient::response r = RestClient::get(url.str(), headers);
  if (r.code == 200) {
    auto x = crow::json::load(r.body);
    std::string result(x["current_observation"]["weather"].s());
    std::cout << result << std::endl;
    return result;
  } else {
    std::cerr << "Unable to get weather data" << std::endl;
    return "error";
  }
}

// Function called by worker thread for device communication
void runner(Devices& devices, std::time_t& alarmTime) {
  for (;;) {
    devices.display_time();
    devices.adjust_brightness();

    if (time_for_alarm(alarmTime)) {
      alarmRinging = true;
      devices.message(get_weather());
    }

    if (alarmRinging) {
      if ( devices.button->value() ) {
        alarmRinging = false;
        devices.stop_buzzing();

        // log how long it took to wake up
        log_wakeup();

        // set alarm time to tomorrow
        struct tm* timeinfo;
        timeinfo = localtime(&alarmTime);
        timeinfo->tm_mday++;
        alarmTime = mktime(timeinfo);
        std::cerr << "New alarm time: " << alarmTime << std::endl;

      } else {
        devices.start_buzzing();
      }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
}

Devices devices;

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
  std::thread t1(runner, std::ref(devices), std::ref(alarmTime));

  // define web server & routes
  crow::SimpleApp app;

  CROW_ROUTE(app, "/")
  ([](const crow::request& req) {
    bool timeChanged = false;
    std::time_t newTime = std::time(NULL);
    struct tm* timeinfo;
    timeinfo = localtime(&newTime);

    if(req.url_params.get("hour") != nullptr) {
      double hour = boost::lexical_cast<double>(req.url_params.get("hour"));
      timeinfo->tm_hour = hour;
      timeChanged = true;
    }

    if(req.url_params.get("minute") != nullptr) {
      double minute = boost::lexical_cast<double>(req.url_params.get("minute"));
      timeinfo->tm_min = minute;
      timeChanged = true;
    }

    if(req.url_params.get("second") != nullptr) {
      double second = boost::lexical_cast<double>(req.url_params.get("second"));
      timeinfo->tm_sec = second;
      timeChanged = true;
    }

    if (timeChanged) {
      alarmTime = mktime(timeinfo);
      std::cerr << "Set alarm time: " << alarmTime << std::endl;
    }

    std::stringstream text;
    text << index_html;
    return text.str();
  });

  CROW_ROUTE(app, "/alarm.json")
  ([]() {
    struct tm* timeinfo;
    timeinfo = localtime(&alarmTime);

    crow::json::wvalue a;
    a["hours"] = timeinfo->tm_hour;
    a["minutes"] = timeinfo->tm_min;
    a["seconds"] = timeinfo->tm_sec;
    return a;
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
