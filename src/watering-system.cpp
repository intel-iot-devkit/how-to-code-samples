#include <string>
using std::string;
#include <sstream>
#include <signal.h>

#include <ctime>
#include <time.h>
#include <vector>
using std::vector;

#include <mraa.hpp>
#include <grovewfs.h>
#include <grovemoisture.h>

#include "../lib/restclient-cpp/include/restclient-cpp/restclient.h"

#include "../lib/crow/crow_all.h"
#include "html.h"
#include "styles.h"

#include "../lib/twilio-cplusplus/Utils.h"
#include "../lib/twilio-cplusplus/Rest.h"
#include "../lib/twilio-cplusplus/TwiML.h"
using namespace twilio;
const string TWILIO_API_VERSION = "2010-04-01";

using namespace std;

// An individual schedule item for a 1 hour time period
struct WateringScheduleItem
{
  bool on;
  bool off;
};

// The entire watering schedule
struct WateringSchedule
{
  WateringScheduleItem schedule[24];
  bool turnedOn;
  bool turnedOff;

  // Initialization function
  WateringSchedule() {
    turnedOn = false;
    turnedOff = false;

    for (int i= 0; i < 24; i++) {
      schedule[i].on = false;
      schedule[i].off = false;
    }
  };

  // Sets the watering schedule for a specific one hour period
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

  // Render the current watering schedule as JSON
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
  std::list<MoistureDataItem> data;
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

    for (std::list<MoistureDataItem>::iterator it = data.begin(); it != data.end(); ++it) {
      resultStream << it->render() << '\n';
    }

    return resultStream.str();
  }
};

// Send sms message using Twilio api
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
  vars.push_back(Var("Body", "Watering System Alert!"));
  response = t.request("/" + TWILIO_API_VERSION + "/Accounts/" + getenv("TWILIO_SID") + "/SMS/Messages", "POST", vars);
  cout << "SMS Sent" << endl;
  cout << response << endl;
}

// Log the event to the remote datastore
void log(const std::string& event) {
  std::cerr << event << std::endl;

  if (!getenv("SERVER") || !getenv("AUTH_TOKEN")) {
    return;
  }

  std::time_t now = std::time(NULL);
  char mbstr[sizeof "2011-10-08T07:07:09Z"];
  std::strftime(mbstr, sizeof(mbstr), "%FT%TZ", std::localtime(&now));

  std::stringstream payload;
  payload << "{\"value\":";
  payload << "\"" << event << mbstr << "\"}";

  RestClient::headermap headers;
  headers["X-Auth-Token"] = getenv("AUTH_TOKEN");

  RestClient::response r = RestClient::put(getenv("SERVER"), "text/json", payload.str(), headers);
  std::cout << "Datastore called. Result:" << r.code << std::endl;
}

// The hardware devices that the example is going to connect to
struct Devices
{
  upm::GroveWFS* flow;
  upm::GroveMoisture* moisture;
  mraa::Gpio* pump;

  int moistureReading = 0;
  bool turnedOn = false;
  bool turnedOff = false;

  Devices() {
  };

  // Initialization function
  void init() {
    // water flow sensor to D2
    flow = new upm::GroveWFS(2);
    flow->clearFlowCounter();
    flow->startFlowCounter();

    // pump attached to D4
    pump = new mraa::Gpio(4);
    pump->dir(mraa::DIR_OUT);
    pump->write(0);

    // moisture sensor attached to A1
    moisture = new upm::GroveMoisture(1);
  };

  // Cleanup on exit
  void cleanup() {
    flow->stopFlowCounter();
    delete flow;
    delete pump;
    delete moisture;
  }

  // Reads the current value from the moisture sensor
  int readMoisture() {
    return moisture->value();
  }

  // Reads the current value from the flow sensor
  int readFlow() {
    return flow->flowRate();
  }

  // Is the water supposed to be turned on?
  bool turned_on() {
    return turnedOn;
  }

  // Is the water supposed to be turned off?
  bool turned_off() {
    return turnedOff;
  }

  // Turn on the water
  void turn_on() {
    if (turnedOn) return;
    pump->write(1);
    turnedOn = true;
    turnedOff = false;
    message("on");
    log("on");
  }

  // Turn off the water
  void turn_off() {
    if (turnedOff) return;
    pump->write(0);
    turnedOn = false;
    turnedOff = true;
    message("off");
    log("off");
  }

  // Displays message to console
  void message(string msg){
    cout << msg << endl;
  }
};

// The thread that reads the moisture sensor
void runner(Devices& devices, MoistureData& moistureData) {
  for (;;)
  {
    std::time_t now = std::time(NULL);
    char mbstr[sizeof "2011-10-08T07:07:09Z"];
    std::strftime(mbstr, sizeof(mbstr), "%FT%TZ", std::localtime(&now));

    int moistureReading = devices.readMoisture();
    moistureData.add(moistureReading, mbstr);
    sleep(10);
  }
}

// The thread that reads the flow sensor
void runner2(Devices& devices) {
  for (;;) {
    int flowRate = devices.readFlow();
    if((devices.turned_on() && flowRate < 1) || (devices.turned_off() && flowRate > 0)) {
      log("watering system alert");
      send_sms();
      sleep(300);
    }
    sleep(2);
  }
}

// The thread that runs the scheduled tasks
void runner3(Devices& devices, WateringSchedule& schedule) {
  for (;;) {
    time_t currentTime ;
    time(&currentTime);
    struct tm* timeinfo;
    timeinfo = localtime(&currentTime);

    if (schedule.it_is_hour(timeinfo)) {
      if (schedule.is_on_time(timeinfo)) {
        devices.turn_on();
      } else if (schedule.is_off_time(timeinfo)) {
        devices.turn_off();
      }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
}

Devices devices;
WateringSchedule schedule;
MoistureData moistureData;

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

  devices.init();

  std::thread t1(runner, ref(devices), ref(moistureData));
  std::thread t2(runner2, ref(devices));
  std::thread t3(runner3, ref(devices), ref(schedule));

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
    page.replace(f, std::string("$MOISTUREDATA$").length(), mst);
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
    std::stringstream text;
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
