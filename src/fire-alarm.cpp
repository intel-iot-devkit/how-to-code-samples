#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <ctime>
#include <vector>

#include <grove.h>
#include <buzzer.h>
#include <jhd1313m1.h>

#include "../lib/restclient-cpp/include/restclient-cpp/restclient.h"

using namespace std;
#include "../lib/twilio-cplusplus/Utils.h"
#include "../lib/twilio-cplusplus/Rest.h"
#include "../lib/twilio-cplusplus/TwiML.h"
using namespace twilio;

const int ALARM_THRESHOLD = 28;
const string TWILIO_API_VERSION = "2010-04-01";

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

// call data server to note the alarm status
void notify() {
  if (!getenv("SERVER") || !getenv("AUTH_TOKEN")) {
    std::cerr << "Server not configured." << std::endl;
    return;
  }

  std::time_t now = std::time(NULL);
  char mbstr[sizeof "2011-10-08T07:07:09Z"];
  std::strftime(mbstr, sizeof(mbstr), "%FT%TZ", std::localtime(&now));

  std::stringstream payload;
  payload << "{\"value\":";
  payload << "\"" << mbstr << "\"}";

  RestClient::headermap headers;
  headers["X-Auth-Token"] = getenv("AUTH_TOKEN");

  RestClient::response r = RestClient::put(getenv("SERVER"), "text/json", payload.str(), headers);

  std::cerr << r.code << std::endl;
  std::cerr << r.body << std::endl;
}

struct Devices
{
  upm::GroveTemp* temp;
  upm::Buzzer* buzzer;
  upm::Jhd1313m1* screen;

  Devices(){
  };

  void init() {
    // touch sensor connected to A0 (analog in)
    temp = new upm::GroveTemp(0);

    // buzzer connected to D5 (digital out)
    buzzer = new upm::Buzzer(5);

    // screen connected to the default I2C bus
    screen = new upm::Jhd1313m1(0);

    stop_alarm();
  };

  void cleanup() {
    delete temp;
    delete buzzer;
    delete screen;
  }

  void reset() {
    stop_alarm();
    message("Ready");
  }

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

  void start_alarm() {
    message("Fire alarm!");
    buzzer->playSound(266, 0);
  }

  void stop_alarm() {
    message("           ");
    buzzer->stopSound();
    buzzer->stopSound();
  }

  int temperature() {
    return temp->value();
  }
};

Devices devices;

// Handles ctrl-c or other orderly exits
void exit_handler(int param)
{
  devices.cleanup();
  exit(1);
}

int main()
{
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
