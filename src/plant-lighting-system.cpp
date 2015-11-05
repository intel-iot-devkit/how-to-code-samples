#include <string>
using std::string;
#include <sstream>
#include <signal.h>

#include <ctime>
#include <time.h>
#include <vector>
using std::vector;

#include "grove.h"
#include <jhd1313m1.h>
#include <grovemoisture.h>

#include "../lib/restclient-cpp/include/restclient-cpp/restclient.h"

#include "../lib/crow/crow_all.h"
#include "html.h"

#include "../lib/twilio-cplusplus/Utils.h"
#include "../lib/twilio-cplusplus/Rest.h"
#include "../lib/twilio-cplusplus/TwiML.h"
using namespace twilio;
const string TWILIO_API_VERSION = "2010-04-01";

using namespace std;

struct LightScheduleItem
{
  bool on;
  bool off;
};

struct LightSchedule
{
  LightScheduleItem schedule[24];
  bool turnedOn;
  bool turnedOff;

  LightSchedule() {
    turnedOn = false;
    turnedOff = false;

    for (int i= 0; i < 24; i++) {
      schedule[i].on = false;
      schedule[i].off = false;
    }
  };

  void set(int hour, bool on, bool off) {
    schedule[hour].on = on;
    schedule[hour].off = off;
  }

  // useful function used for checking how much time is left before target time
  double countdown(std::time_t& target) {
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    return std::difftime(mktime(timeinfo), target);
  }

  // is it top of the hour?
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

  bool is_on_time(struct tm* timeinfo) {
    return schedule[timeinfo->tm_hour].on;
  }

  bool is_off_time(struct tm* timeinfo) {
    return schedule[timeinfo->tm_hour].off;
  }

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

struct MoistureDataItem
{
  int data;
  string when;
};

struct MoistureData
{
  MoistureDataItem data[20];
  int lastReading;

  MoistureData() {
    lastReading = 0;
  };

  // push this new value onto array
  void add(int val, string timestamp) {

  }

  // nicely formatted text for the web page
  string renderText(){
    // TODO: return entire array
    string result;
    result = to_string(lastReading);
    return result;
  }
};

//send sms message using twilio api
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
  vars.push_back(Var("Body", "Lighting Alert!"));
  response = t.request("/" + TWILIO_API_VERSION + "/Accounts/" + getenv("TWILIO_SID") + "/SMS/Messages", "POST", vars);
  cout << "SMS Sent" << endl;
  cout << response << endl;
}

// call data server to log the event
void log(const std::string& event) {
  std::cerr << event << std::endl;

  if (!getenv("SERVER") || !getenv("AUTH_TOKEN")) {
    std::cerr << "Datastore not configured." << std::endl;
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

  cout << "Datastore updated." << endl;
}

struct Devices
{
  upm::GroveLight* light;
  upm::Jhd1313m1* screen;
  upm::GroveMoisture* moisture;
  int moistureReading = 0;
  int lightReading = 0;
  bool turnedOn = false;
  bool turnedOff = false;

  Devices() {
  };

  void init() {
    // screen connected to the default I2C bus
    screen = new upm::Jhd1313m1(0, 0x3E, 0x62);

    // Light sensor attached to A0
    light = new upm::GroveLight(0);

    // moisture sensor attached to A1
    moisture = new upm::GroveMoisture(1);
  };

  int readMoisture() {
    return moisture->value();
  }

  int readLight() {
    return light->value();
  }

  bool turned_on() {
    return turnedOn;
  }

  bool turned_off() {
    return turnedOff;
  }

  void turn_on() {
    if (turnedOn) return;
    turnedOn = true;
    turnedOff = false;
    message("on");
    log("on");
  }

  void turn_off() {
    if (turnedOff) return;
    turnedOn = false;
    turnedOff = true;
    message("off");
    log("off");
  }

  void display_time(struct tm* timeinfo) {
    char buffer[80];
    strftime(buffer, 80, "%I:%M:%S", timeinfo);
    std::string str(buffer);

    message(str, 0x00ff00);
  }

  void cleanup() {
    delete screen;
    delete moisture;
    delete light;
  }

  void message(const std::string& input, const std::size_t color = 0x0000ff) {
    std::size_t red = (color & 0xff0000) >> 16;
    std::size_t green = (color & 0x00ff00) >> 8;
    std::size_t blue = (color & 0x0000ff);

    std::string text(input);
    text.resize(16, ' ');

    screen->setCursor(0, 0);
    screen->write(text);
    screen->setColor(red, green, blue);
  }
};

// The thread that reads the moisture sensor
void runner(Devices& devices, MoistureData& moistureData) {
  for (;;)
  {
    int moistureReading = devices.readMoisture();
    moistureData.add(moistureReading, "TIMESTAMPHERE");
    sleep(10000);
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
        std::this_thread::sleep_for(std::chrono::milliseconds(600000));
    } else {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
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
      } else if (schedule.is_off_time(timeinfo)) {
        devices.turn_off();
      }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
}

Devices devices;
LightSchedule schedule;
MoistureData moistureData;

// Handles ctrl-c or other orderly exits
void exit_handler(int param)
{
  devices.cleanup();
  exit(1);
}

// The main function for the example.
int main()
{
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

  app.port(3000).multithreaded().run();
  t1.join();
  t2.join();
  t3.join();

  return 0;
}
