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
 * @brief Smart stove top in C++
 *
 * This smart-stovetop application is part of a series of how-to Intel IoT code
 * sample exercises using the Intel® IoT Developer Kit, Intel® Edison board,
 * cloud platforms, APIs, and other technologies.
 *
 * @hardware Sensors used:\n
 * Grove Speaker\n
 * Grove Flame Sensor\n
 * Grove IR Temperature Sensor\n
 *
 * @cc
 * @cxx -std=c++1y
 * @ld -lupm-grovespeaker -lupm-yg1006 -lupm-otp538u -lpaho-mqtt3cs -lupm-grove -lboost_system -lboost_thread -lboost_filesystem -lboost_date_time -lpthread -lcurl
 *
 * Additional source files required to build this example:
 * @req datastore.cpp
 * @req mqtt.cpp
 *
 * @date 09/22/2016
 */
#include <iostream>
#include <string>
#include <signal.h>

#include "../lib/crow/crow_all.h"
#include "html.h"
#include "styles.h"

using namespace std;

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

// Global temp value for app; will change when you use app
float myTemp = 1000;

// Send notification to remote datastore
void notify() {
	time_t now = std::time(NULL);
	char mbstr[sizeof "2011-10-08T07:07:09Z"];
	strftime(mbstr, sizeof(mbstr), "%FT%TZ", localtime(&now));

	stringstream text;
	text << "{\"value\":";
	text << "\"" << mbstr << "\"}";

	log_mqtt(text.str());
	log_datastore(text.str());
}

Devices devices;

// Reads the flame sensor every 1 second
void senseFlame() {
	for(;;) {
		bool val = devices.flameDetected();
		if (val) {
			cout << "Flame detected." << endl;
			devices.alarm();
		}

		else
		{
			cout << "No flame detected." << endl;
			devices.stopAlarm();
			sleep(1);
		}
	}
}

// Continously reads the IR temperature sensor every 1 minute for
// both object and ambient temperature, and compares the two.
// Sounds alarm if object temp is higher than set temp.
void senseTemp(){
	for(;;) {
		float temp1 = devices.objectTemperature();

		cout << "Temp: " << std::fixed << setprecision(2)
				 << temp1
				 << " C" << endl;

		if(temp1 >= myTemp){
			devices.alarm();
			notify();
		}
		else
			sleep(60);
	}
}

// Keeps the flame sensor running
void runFlame(Devices& devices) {
  senseFlame();
}

// Keeps the ir temperature sensor running
void runTemp(Devices& devices) {
  senseTemp();
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
    stringstream text;
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
