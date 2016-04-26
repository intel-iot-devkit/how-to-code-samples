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
 * @brief Earthquake detector in C++
 *
 * This earthquake-detector application is part of a series of how-to Intel IoT code
 * sample exercises using the IntelÂ® IoT Developer Kit, IntelÂ® Edison board,
 * cloud platforms, APIs, and other technologies.
 *
 * @hardware Sensors used:\n
 * Grove 3 Axis Digital Accelerometer\n
 * Grove RGB LCD Screen\n
 *
 * @cc
 * @cxx -std=c++1y
 * @ld -lupm-mma7660 -lupm-i2clcd -lcurl
 *
 * Additional source files required to build this example:
 *
 * @date 04/04/2016
 */

#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <ctime>
#include <signal.h>

#include <mma7660.hpp>
#include <jhd1313m1.h>

#include "../lib/restclient-cpp/include/restclient-cpp/restclient.h"
#include "../lib/json-cpp/json/json.h"

const std::string DEFAULT_LATITUDE = "47.641944";
const std::string DEFAULT_LONGITUDE = "-122.127222";

// Convenience function to return the configured latitude
std::string latitude() {
  if (getenv("LATITUDE")) {
    return getenv("LATITUDE");
  } else {
    return DEFAULT_LATITUDE;
  }
}

// Convenience function to return the configured latitude
std::string longitude() {
  if (getenv("LONGITUDE")) {
    return getenv("LONGITUDE");
  } else {
    return DEFAULT_LONGITUDE;
  }
}

// The hardware devices that the example is going to connect to
struct Devices
{
  upm::MMA7660* accel;
  upm::Jhd1313m1* screen;

  Devices(){
  };

  // Initialization function
  void init() {
    // accelerometer connected to i2c
    accel = new upm::MMA7660(MMA7660_I2C_BUS,
                                MMA7660_DEFAULT_I2C_ADDR);

    accel->setModeStandby();
    accel->setSampleRate(upm::MMA7660::AUTOSLEEP_64);
    accel->setModeActive();

    // screen connected to the default I2C bus
    screen = new upm::Jhd1313m1(0);
  };

  // Cleanup on exit
  void cleanup() {
    delete accel;
    delete screen;
  }

  // Reset the display
  void reset() {
    message("quakebot ready");
  }

  // Display message when checking for earthquake
  void checking() {
    message("checking...");
  }

  // Display message when there really has been a recent earthquake
  void warning() {
    message("Earthquake!");
  }

  // Display message when a false alarm
  void noquake() {
    message("No quake.");
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

  // Read the current data from the accelerometer
  void getAcceleration(float* x, float* y, float* z) {
    accel->getAcceleration(x, y, z);
  }
};

// Make a REST API call to the USGS to see if there has been
// a recent earthquake detected in the local area
void verify(Devices* devices) {
  devices->checking();

  // we'll check for quakes in the last ten minutes
  std::time_t base = std::time(NULL);
  struct tm* tm = localtime(&base);
  tm->tm_min -= 10;
  time_t next = mktime(tm);
  char mbstr[sizeof "2011-10-08T07:07:09Z"];
  std::strftime(mbstr, sizeof(mbstr), "%FT%TZ", std::localtime(&next));

  std::stringstream query;
  query << "http://earthquake.usgs.gov/fdsnws/event/1/query?";
  query << "format=geojson&";
  query << "starttime=" << mbstr << "&";
  query << "latitude=" << latitude() << "&";
  query << "longitude=" << longitude() << "&";
  query << "maxradiuskm=500";

  std::cerr << query.str();

  RestClient::response r = RestClient::get(query.str());

  Json::Value root;
  std::istringstream str(r.body);
  str >> root;

  const Json::Value features = root["features"];
  if (features.size() > 0) {
    devices->warning();
  } else {
    devices->noquake();
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

  bool motionDetected = false;
  bool prev = false;
  float ax = 0, ay = 0, az = 0;

  for (;;) {
    devices.getAcceleration(&ax, &ay, &az);
    motionDetected = (ax > 1.0 || ay > 1.0 || az > 1.0);

    if (motionDetected && !prev) { verify(&devices); }
    prev = motionDetected;
    sleep(1);
  }

  return MRAA_SUCCESS;
}
