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
 * @brief Home fall tracker in C++
 *
 * This fall-tracker application is part of a series of how-to Intel IoT code
 * sample exercises using the IntelÂ® IoT Developer Kit, IntelÂ® Edison board,
 * cloud platforms, APIs, and other technologies.
 *
 * @hardware Sensors used:\n
 * Grove OLED Display 0.96"\n
 * Grove 3 Axis Digital accelerometer (16g)\n
 *
 * @cc
 * @cxx -std=c++1y
 * @ld -lupm-i2clcd -lupm-adxl345 -lssl -lcrypto -lpaho-mqtt3cs -lcurl
 *
 * Additional source files required to build this example:
 * @req datastore.cpp
 * @req mqtt.cpp
 *
 * @date 04/04/2016
 */
#include <string>
using std::string;
#include <sstream>
#include <iostream>
#include <signal.h>

#include <ctime>
#include <time.h>
#include <vector>
using std::vector;

#include <ssd1308.h>
#define OLED_DEVICE_ADDRESS    0x3C
#define OLED_BUS_NUMBER        0x0
#include <adxl345.hpp>

#include "../lib/restclient-cpp/include/restclient-cpp/restclient.h"

#include "datastore.h"
#include "mqtt.h"

#include "../lib/twilio-cplusplus/Utils.h"
#include "../lib/twilio-cplusplus/Rest.h"
#include "../lib/twilio-cplusplus/TwiML.h"
using namespace twilio;
const string TWILIO_API_VERSION = "2010-04-01";

using namespace std;

// Send sms message using Twilio api
void send_sms() {
  if (!getenv("TWILIO_SID") || !getenv("TWILIO_TOKEN") ||
      !getenv("TWILIO_TO") || !getenv("TWILIO_FROM")) {
    cerr << "Twilio not configured." << endl;
    return;
  }

  string response;
  vector<Var> vars;

  Rest t(getenv("TWILIO_SID"), getenv("TWILIO_TOKEN"));

  vars.push_back(Var("To", getenv("TWILIO_TO")));
  vars.push_back(Var("From", getenv("TWILIO_FROM")));
  vars.push_back(Var("Body", "Fall detected!"));
  response = t.request("/" + TWILIO_API_VERSION + "/Accounts/" + getenv("TWILIO_SID") + "/SMS/Messages", "POST", vars);
  cout << "SMS Sent" << endl;
  cout << response << endl;
}

// Log the event to the remote datastore
void log(const string& event) {
  cout << event << endl;

  time_t now = time(NULL);
  char mbstr[sizeof "2011-10-08T07:07:09Z"];
  strftime(mbstr, sizeof(mbstr), "%FT%TZ", localtime(&now));

  stringstream text;
  text << "{\"value\":";
  text << "\"" << event << mbstr << "\"}";

  log_mqtt(text.str());
  log_datastore(text.str());
}

// The hardware devices that the example is going to connect to
struct Devices
{
  upm::SSD1308 *screen;
  upm::Adxl345 *accel;

  float previousX, previousY, previousZ = 0;
  float currentX, currentY, currentZ = 0;

  Devices(){
  };

  // Initialization function
  void init() {
    // screen connected to the default I2C bus
    screen = new upm::SSD1308(OLED_BUS_NUMBER, OLED_DEVICE_ADDRESS);

    // accelerometer also connected via I2C
    accel = new upm::Adxl345(0);
  };

  // Cleanup on exit
  void cleanup() {
    delete screen;
    delete accel;
  }

  // Display a message on the OLED
  void message(const string& input) {
    string text(input);
    text.resize(16, ' ');

    screen->clear();
    screen->home();
    screen->write(text);
  }

  // Reads the current value from accelerometer
  void read_accelerometer() {
    float* acc;
    accel->update();
    acc = accel->getAcceleration();
    currentX = acc[0];
    currentY = acc[1];
    currentZ = acc[2];
  }

  // Checks to see if it looks like the wearer has fallen down
  bool fall_detected() {
    if (previousX - currentX >= 5) {
      fprintf(stdout, "PX: %5.2f g\n", previousX);
      fprintf(stdout, "AccX: %5.2f g\n", currentX);
      return true;
    }
    if (previousY - currentY >= 5) {
      fprintf(stdout, "PY: %5.2f g\n", previousY);
      fprintf(stdout, "AccY: %5.2f g\n", currentY);
      return true;
    }
    if (previousZ - currentZ >= 5) {
      fprintf(stdout, "PZ: %5.2f g\n", previousZ);
      fprintf(stdout, "AccZ: %5.2f g\n", currentZ);
      return true;
    }
    return false;
  }

  void save_values() {
    previousX = currentX;
    previousY = currentY;
    previousZ = currentZ;
  }
};

Devices devices;

void alert(){
  devices.message("Fall Detected!");
  log("Fall detected");
  send_sms();
}

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

  for (;;) {
    devices.read_accelerometer();
    if (devices.fall_detected()) {
      alert();
    }

    devices.save_values();
    usleep(250);
  }

  return MRAA_SUCCESS;
}
