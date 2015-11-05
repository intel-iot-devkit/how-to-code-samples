#include <iostream>
#include <string>

#include <grove.h>
#include <yg1006.h>
#include <otp538u.h>
#include <GroveSpeaker.h>

#include "../lib/restclient-cpp/include/restclient-cpp/restclient.h"
#include "../lib/crow/crow_all.h"
#include "html.h"

#define OTP538U_AREF 5.0

using namespace std;

// Global temp value for app; will change when you use app
float myTemp = 1000;

// Send notification to datastore
void notify() {
  if (!getenv("SERVER") || !getenv("AUTH_TOKEN")) {
    cerr << "Server not configured." << std::endl;
    return;
  }

  RestClient::headermap headers;
  headers["X-Auth-Token"] = getenv("AUTH_TOKEN");
  RestClient::response r = RestClient::get(getenv("SERVER"), headers);
  cerr << r.code << std::endl;
  cerr << r.body << std::endl;
}

// TODO
struct Devices
{
  upm::GroveSpeaker* speaker;
  upm::YG1006* flame;
  upm::OTP538U* temps;

  Devices(){
  };

  // TODO
  void init() {
    // speaker connected to D5 (digital out)
    speaker = new upm::GroveSpeaker(5);

    // flame sensor on D4
    flame = new upm::YG1006(4);

    // Instantiate a OTP538U on analog pins A0 and A1
    // A0 is used for the Ambient Temperature and A1 is used for the
    // Object temperature.
    // only plug ir temp sensor into A0 with this code
    temps = new upm::OTP538U(0, 1, OTP538U_AREF);
  }

  // TODO
  void cleanup() {
    delete speaker;
    delete flame;
    delete temps;
  }

  // Starts the alarm
  void alarm() {
    speaker->playSound('c', true, "high");
  }

  // Reads the flame sensor every 1s
  void senseFlame() {
    for(;;) {
      bool val = flame->flameDetected();
      if (val) {
        cout << "Flame detected." << endl;
        alarm();
      }

      else
      {
        cout << "No flame detected." << endl;
        sleep(1);
      }
    }
  }

  // Continously reads the IR temperature sensor every 1 minute for
  // both object and ambient temperature, and compares the two.
  // Sounds alarm if object temp is higher than set temp.
  void senseTemp(){
    for(;;) {
      cout << "Ambient temp: " << std::fixed << setprecision(2)
           << temps->ambientTemperature()
           << " C, Object temp: " << temps->objectTemperature()
           << " C" << endl;

      float temp1 = temps->objectTemperature();

      if(temp1 >= myTemp){
        alarm();
        notify();
      }
      else
        sleep(60);
    }
  }
};

// Keeps the flame sensor running
void runFlame(Devices& devices) {
  devices.senseFlame();
}

// Keeps the ir temperature sensor running
void runTemp(Devices& devices) {
  devices.senseTemp();
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
    cerr << "ERROR: Unsupported platform" << endl;
    return MRAA_ERROR_INVALID_PLATFORM;
  }

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

  // start web server
  app.port(3000).multithreaded().run();

  // all done
  t1.join();
  t2.join();

  return 0;
}
