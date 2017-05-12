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
 * @brief Plant lighting system in C++
 *
 * This plant-lighting-system application is part of a series of how-to Intel IoT code
 * sample exercises using the Intel® IoT Developer Kit, Intel® Edison board,
 * cloud platforms, APIs, and other technologies.
 *
 * @hardware Sensors used:\n
 * Grove Light Sensor\n
 * Grove Moisture Sensor\n
 * Grove RGB LCD\n
 *
 * @cc
 * @cxx -std=c++1y
 * @ld -lupm-i2clcd -lupm-jhd1313m1 -lupm-grovemoisture -lupm-ldt0028 -lpaho-mqtt3cs -lupm-grove -lboost_system -lboost_thread -lboost_filesystem -lboost_date_time -lpthread -lssl -lcrypto -lcurl
 *
 * Additional source files required to build this example:
 * @req datastore.cpp
 * @req mqtt.cpp
 *
 * @date 09/22/2016
 */

#include <string>
using std::string;
#include <sstream>
#include <signal.h>

#include <ctime>
#include <time.h>
#include <vector>
using std::vector;

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
#include "services/services.h"

#include "../lib/twilio-cplusplus/Utils.h"
#include "../lib/twilio-cplusplus/Rest.h"
#include "../lib/twilio-cplusplus/TwiML.h"
using namespace twilio;
const string TWILIO_API_VERSION = "2010-04-01";

// An individual schedule item for a 1 hour time period
struct LightScheduleItem
{
  bool on;
  bool off;
};

// The entire lighting schedule
struct LightSchedule
{
  LightScheduleItem schedule[24];
  bool turnedOn;
  bool turnedOff;

  // Initialization function
  LightSchedule() {
    turnedOn = false;
    turnedOff = false;

    for (int i= 0; i < 24; i++) {
      schedule[i].on = false;
      schedule[i].off = false;
    }
  };

  // Sets the lighting schedule for a specific one hour period
  void set(int hour, bool on, bool off) {
    schedule[hour].on = on;
    schedule[hour].off = off;
  }

  // Useful function used for checking how much time is left before target time
  double countdown(std::time_t& target) {
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    return std::difftime(mktime(timeinfo), target);
  }

  // Is it top of the hour?
  bool it_is_hour(struct tm* timeinfo) {
    time_t alarmTime ;
    timeinfo->tm_min = 0;
    timeinfo->tm_sec = 0;
    alarmTime = mktime(timeinfo);

    double remaining = countdown(alarmTime);

    if (remaining > 0 && remaining < 5) {
      return true;
    } else return false;
  }

  // Is it time to turn on?
  bool is_on_time(struct tm* timeinfo) {
    return schedule[timeinfo->tm_hour].on;
  }

  // Is it time to turn off?
  bool is_off_time(struct tm* timeinfo) {
    return schedule[timeinfo->tm_hour].off;
  }

  // Render the current lighting schedule as JSON
  crow::json::wvalue renderJSON() {
    crow::json::wvalue result;
    for (int i= 0; i < 24; i++) {
      string iStr = to_string(i);
      result["data"][iStr]["on"] = schedule[i].on ? 1 : 0;
      result["data"][iStr]["off"] = schedule[i].off ? 1 : 0;
    }

    return result;
  }
};

// A single data reading for the connected moisture sensor
struct MoistureDataItem
{
  int data;
  string when;

  // Initialization function
  MoistureDataItem(int d, string w) {
    data = d;
    when = w;
  };

  string render() {
    stringstream resultStream;
    resultStream << "<tr>";
    resultStream << "<td>" << when << "</td>";
    resultStream << "<td>" << data << "</td>";
    resultStream << "</tr>";
    return resultStream.str();
  }
};

// All of the data reads from the moisture sensor
struct MoistureData
{
  list<MoistureDataItem> data;
  int lastReading;

  // Initialization function
  MoistureData() {
    lastReading = 0;
  };

  // Push this new value onto list
  void add(int val, string timestamp) {
    MoistureDataItem item(val, timestamp);
    data.push_front(item);
    if (data.size() > 20) {data.pop_back();}
  }

  // Nicely formatted text for the web page
  string renderText() {
    stringstream resultStream;

    for (list<MoistureDataItem>::iterator it = data.begin(); it != data.end(); ++it) {
      resultStream << it->render() << '\n';
    }

    return resultStream.str();
  }
};

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
  vars.push_back(Var("Body", "Lighting Alert!"));
  response = t.request("/" + TWILIO_API_VERSION + "/Accounts/" + getenv("TWILIO_SID") + "/SMS/Messages", "POST", vars);
  cout << "SMS Sent" << endl;
  cout << response << endl;
}

// Log the event to the remote datastore
void log(const string& event) {
  cerr << event << endl;

  time_t now = time(NULL);
  char mbstr[sizeof "2011-10-08T07:07:09Z"];
  strftime(mbstr, sizeof(mbstr), "%FT%TZ", localtime(&now));

  stringstream text;
  text << "{\"value\":";
  text << "\"" << event << mbstr << "\"}";

  log_service(text.str());
  log_datastore(text.str());
}

Devices devices;
LightSchedule schedule;
MoistureData moistureData;

// The thread that reads the moisture sensor
void runner(Devices& devices, MoistureData& moistureData) {
  for (;;)
  {
    time_t now = time(NULL);
    char mbstr[sizeof "2011-10-08T07:07:09Z"];
    strftime(mbstr, sizeof(mbstr), "%FT%TZ", localtime(&now));

    int moistureReading = devices.readMoisture();
    moistureData.add(moistureReading, mbstr);
    sleep(10);
  }
}

// The thread that reads the light sensor
void runner2(Devices& devices) {
  for (;;)
  {
    int lightReading = devices.readLight();

    if ((lightReading < 2 && devices.turned_on()) ||
        (lightReading > 4 && devices.turned_off())) {
        log("Lighting alert");
        devices.message("Lighting alert");
        send_sms();
        this_thread::sleep_for(chrono::milliseconds(600000));
    } else {
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
  }
}

// The thread that runs the scheduled tasks
void runner3(Devices& devices, LightSchedule& schedule) {
  for (;;) {
    time_t currentTime ;
    time(&currentTime);
    struct tm* timeinfo;
    timeinfo = localtime(&currentTime);

    devices.display_time(timeinfo);

    if (schedule.it_is_hour(timeinfo)) {
      if (schedule.is_on_time(timeinfo)) {
        devices.turn_on();
        log("on");
      } else if (schedule.is_off_time(timeinfo)) {
        devices.turn_off();
        log("off");
      }
    }

    this_thread::sleep_for(chrono::milliseconds(1000));
  }
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

  devices.init();

  thread t1(runner, ref(devices), ref(moistureData));
  thread t2(runner2, ref(devices));
  thread t3(runner3, ref(devices), ref(schedule));

  crow::SimpleApp app;

  CROW_ROUTE(app, "/")
  ([](const crow::request& req) {
    stringstream text;
    text << index_html;

    string page;
    page = text.str();
    string mst;
    mst = moistureData.renderText();
    size_t f = page.find("$MOISTUREDATA$");
    page.replace(f, string("$MOISTUREDATA$").length(), mst);
    return page;
  });

  CROW_ROUTE(app, "/schedule")
    .methods("PUT"_method, "GET"_method)
    ([](const crow::request& req) {
      if (req.method == "PUT"_method) {
        auto sched = crow::json::load(req.body);
        if (!sched)
           return crow::response(400);

        if (sched.size() != 24) {
          cout << "Invalid schedule data" << endl;
        }

        for (int i= 0; i < 24; i++) {
          string tempI = to_string(i);
          schedule.set(i, sched[tempI]["on"].b(), sched[tempI]["off"].b());
        }

        return crow::response("ok");
      } else {
        return crow::response(schedule.renderJSON());
      }
  });

  CROW_ROUTE(app, "/on")
  ([]() {
    devices.turn_on();
    return crow::response("ok");
  });

  CROW_ROUTE(app, "/off")
  ([]() {
    devices.turn_off();
    return crow::response("ok");
  });

  CROW_ROUTE(app, "/styles.css")
  ([]() {
    stringstream text;
    text << styles_css;
    return text.str();
  });

  // starts the web server
  app.port(3000).multithreaded().run();

  // waits forever on the worker threads
  t1.join();
  t2.join();
  t3.join();

  return 0;
}
