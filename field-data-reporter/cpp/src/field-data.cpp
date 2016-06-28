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
 * @brief Field data recorder in C++
 *
 * This feild-data application is part of a series of how-to Intel IoT code
 * sample exercises using the Intel® IoT Developer Kit, Intel® Edison board,
 * cloud platforms, APIs, and other technologies.
 *
 * @hardware Sensors used:\n
 * Grove OLED Display 0.96"\n
 * Grove QTouch Sensor\n
 * BMP Atmospheric Pressure Sensor
 *
 * @cc
 * @cxx -std=c++1y
 * @ld -lupm-i2clcd -lupm-bmpx8x -lupm-at42qt1070 -lboost_system -lboost_thread -lboost_filesystem -lboost_date_time -lpthread
 *
 * Additional source files required to build this example:
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

#include <ssd1308.h>
#include <at42qt1070.hpp>
#include <bmpx8x.h>
#define OLED_DEVICE_ADDRESS    0x3C
#define OLED_BUS_NUMBER        0x0

#include "../lib/crow/crow_all.h"

using namespace std;

// An individual data item
struct DataItem
{
  uint32_t pressure = 0;
  float temperature = 0;
  float altitude = 0;
  uint32_t sealevel = 0;
  string when;

  DataItem(uint32_t p, float t, float a, uint32_t s) {
    pressure = p;
    temperature = t;
    altitude = a;
    sealevel = s;

    time_t now = std::time(NULL);
    char mbstr[sizeof "2011-10-08T07:07:09Z"];
    strftime(mbstr, sizeof(mbstr), "%FT%TZ", localtime(&now));
    when = mbstr;
  };

  crow::json::wvalue renderJSON() {
    crow::json::wvalue result;

    result["pressure"] = pressure;
    result["temperature"] = temperature;
    result["altitude"] = altitude;
    result["sealevel"] = sealevel;
    return result;
  }

  string renderText() {
    string result;
    result = to_string(pressure);
    return result;
  }
};

// The data that has been collected
struct DataCollection
{
  std::list<DataItem> data;

  // Adds a new data item to the collection
  void add(uint32_t p, float t, float a, uint32_t s) {
    DataItem item(p, t, a, s);
    add(item);
  }

  // Adds a new data item to the collection
  void add(DataItem di) {
    data.push_front(di);
    if (data.size() > 100) {data.pop_back();}
  }

  // Returns JSON
  crow::json::wvalue renderJSON() {
    crow::json::wvalue result;

    for (std::list<DataItem>::iterator it = data.begin(); it != data.end(); ++it) {
      result[it->when] = it->renderJSON();
    }

    return result;
  }

  DataItem most_recent() {
    return data.front();
  }
};

// The hardware devices that the example is going to connect to
struct Devices
{
  upm::SSD1308 *screen;
  upm::AT42QT1070 *touch;
  upm::BMPX8X *barometer;

  Devices(){
  };

  // Initialization function
  void init() {
    // screen connected to the default I2C bus
    screen = new upm::SSD1308(OLED_BUS_NUMBER, OLED_DEVICE_ADDRESS);

    // touch sensor aka buttons
    touch = new upm::AT42QT1070(AT42QT1070_I2C_BUS, AT42QT1070_DEFAULT_I2C_ADDR);

    // barometer
    barometer = new upm::BMPX8X(0, 0x77);
  };

  // Cleanup on exit
  void cleanup() {
    delete screen;
    delete touch;
    delete barometer;
  }

  // Display a message on the OLED
  void message(const string& input) {
    string text(input);
    text.resize(16, ' ');

    screen->clear();
    screen->home();
    screen->write(text);
  }

  bool button_pressed() {
    touch->updateState();

    if (touch->getButtons() > 0)
      return true;
    else
      return false;
  }

  DataItem read_barometer() {
    uint32_t p = 0;
    float t = 0;
    float a = 0;
    uint32_t s = 0;

    p = barometer->getPressure();
    t = barometer->getTemperature();
    a = barometer->getAltitude();
    s = barometer->getSealevelPressure();

    DataItem di(p, t, a, s);
    return di;
  }
};

// Function called by worker thread for device communication
void runner(Devices& devices, DataCollection& data) {
  for (;;) {
    if ( devices.button_pressed() ) {
      devices.message(data.most_recent().renderText());
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
}

void runner2(Devices& devices, DataCollection& data) {
  for (;;) {
    data.add(devices.read_barometer());
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
}

Devices devices;
DataCollection data;

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
  std::thread t1(runner, std::ref(devices), std::ref(data));
  std::thread t2(runner2, std::ref(devices), std::ref(data));

  // define web server & routes
  crow::SimpleApp app;

  CROW_ROUTE(app, "/")
  ([]() {
    return crow::response(data.renderJSON());
  });

  // start web server
  app.port(3000).multithreaded().run();

  // wait forever for the thread to exit
  t1.join();
  t2.join();

  return MRAA_SUCCESS;
}
