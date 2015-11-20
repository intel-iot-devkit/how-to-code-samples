#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sstream>
#include <thread>
#include <ctime>
#include <string>
#include <signal.h>

#include <grove.h>
#include <grovespeaker.h>
#include "tp401.h"

#include "../lib/restclient-cpp/include/restclient-cpp/restclient.h"

using namespace std;

struct Devices
{
  upm::GroveSpeaker* speaker;
  upm::TP401* sensor;

  Devices() {
  };

  // Initialization function
  void init() {
  	// speaker connected to digital D5
    speaker = new upm::GroveSpeaker(5);

    // air sensor connected to analog A0
    sensor = new upm::TP401(0);

    // start sensor warmup process
    warmup();
  };

  void alarm() {
    speaker->playSound('a', true, "high");
    speaker->playSound('c', true, "high");
    speaker->playSound('g', true, "high");
  }

  void notify() {
    if (!getenv("SERVER") || !getenv("AUTH_TOKEN")) {
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
    std::cout << "Datastore called. Result:" << r.code << std::endl;
    std::cout << r.body << std::endl;
  }

  void cleanup() {
    delete speaker;
    delete sensor;
  }

  //values and console output
  string airQuality(uint16_t value)
  {
    if(value < 50) return "Fresh Air";
    if(value < 200) return "Normal Indoor Air";
    if(value < 400) return "Low Pollution";
    if(value < 600) return "High Pollution - Action Recommended";
    return "Very High Pollution - Take Action Immediately";
  }

  void warmup(){
    cout << sensor->name() << endl;

    fprintf(stdout, "Heating sensor for 3 minutes...\n");

    // wait 3 minutes for sensor to warm up
    for(int i = 0; i < 3; i++) {
        if(i) {
            fprintf(stdout, "Please wait, %d minute(s) passed..\n", i);
        }
        sleep(60);
    }

    fprintf(stdout, "Sensor ready!\n");
  }

  void airVal(){
    // read raw value
    uint16_t value = sensor->getSample();

    // read CO ppm (can vary slightly from previous read)
    float ppm = sensor->getPPM();
    fprintf(stdout, "raw: %4d ppm: %5.2f   %s\n", value, ppm, airQuality(value).c_str());

    if (value > 400){
    	notify();
    	alarm();
    }

    // sleep for 2.5s
    usleep(2500000);
  }
};

Devices devices;

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

  for (;;) {
  	devices.airVal();
  }

  return MRAA_SUCCESS;
}
