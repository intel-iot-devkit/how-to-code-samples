/*
* Copyright (c) 2017 Intel Corporation.
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
 * @brief Sound detector in C++
 *
 * This sound detector application is part of a series of how-to Intel IoT code
 * sample exercises using the Intel® IoT Developer Kit, Intel® Edison board,
 * cloud platforms, APIs, and other technologies.
 *
 * @hardware Sensors used:\n
 * Grove Sound Sensor\n
 * Grove RGB LCD\n
 * OR\n
 * DFRobot Sound Sensor\n
 * DFRobot LCD Shield
 *
 * @cc
 * @cxx -std=c++1y
 * @ld -lupm-i2clcd -lupm-jhd1313m1 -lump-mic -lupm-ldt0028 -lpaho-mqtt3cs -lcurl
 *
 * Additional source files required to build this example:
 * @req datastore.cpp
 * @req mqtt.cpp
 *
 * @date 02/17/2017
 */

#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <ctime>
#include <signal.h>

using namespace std;

#include "kits.h"
#if INTEL_IOT_KIT == DFROBOTKIT
#include "dfrobotkit.hpp"
#else
#include "grovekit.hpp"
#endif

#include "../lib/restclient-cpp/include/restclient-cpp/restclient.h"

#include "datastore.h"
#include "services/services.h"

// Call remote datastore server to log activity
void notify(int reading) {
  time_t now = time(NULL);
  char mbstr[sizeof "2011-10-08T07:07:09Z"];
  strftime(mbstr, sizeof(mbstr), "%FT%TZ", localtime(&now));

  stringstream text;
  text << "{\"state\":";
  text << "\"" << reading << " " << mbstr << "\"}";

  cout << reading << " " << mbstr << endl;
  log_service(text.str());
  log_datastore(text.str());
}

Devices devices;

void display(int level) {
  std::size_t color = 0x000000;
  if (level < 1) {
    color = 0xffffff;
  } else if (level < 2) {
    color = 0xffff00;
  } else if (level < 3) {
    color = 0x008000;
  } else if (level < 4) {
    color = 0x0000ff;
  } else if (level < 5) {
    color = 0x800080;
  } else {
    color = 0xff0000;
  }

  devices.message(to_string(level), color);
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

  // create and initialize UPM devices
  devices.init();
  devices.reset();

  int level = 0;
  int counter = 0;

  // every 100 ms, check sound level
  for (;;) {
    level = devices.get_mic_level();
    display(level);
    counter++;

    // every 5 seconds, also log
    if ( counter > 59 ) {
      notify(level);
      counter = 0;
    }

    usleep(100000);
  }

  return MRAA_SUCCESS;
}
