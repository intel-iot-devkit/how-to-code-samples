#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sstream>
#include <thread>
#include <string>
#include <signal.h>

#include <grove.h>
#include <ublox6.h>
#include <rfr359f.h>

#include "../lib/restclient-cpp/include/restclient-cpp/restclient.h"

const size_t bufferLength = 256;

// The hardware devices that the example is going to connect to
struct Devices
{
	upm::RFR359F* dInterupter;
	upm::Ublox6* nmea;

  Devices(){
  };


  // Initialization function
  void init() {
	  dInterupter = new upm::RFR359F(2);
	  nmea = new upm::Ublox6(0);
	  initPort();

  };

  // Cleanup on exit
  void cleanup() {
	  delete dInterupter;
	  delete nmea;
  }

  int initPort(){
	  if (!nmea->setupTty(B115200))
	    {
	      std::cerr << "Failed to setup tty port parameters" << std::endl;
	      return 1;
	    }
  }

  void gpsInfo(){
	  char nmeaBuffer[bufferLength];
	      // we don't want the read to block in this example, so always
	      // check to see if data is available first.
      int rv = nmea->readData(nmeaBuffer, bufferLength);

	      if (nmea->dataAvailable())
	        {
	          if (rv > 0)
	            write(1, nmeaBuffer, rv);
	          sleep(1);
	          if (rv < 0) // some sort of read error occured
	            {
	              std::cerr << "Port read error." << std::endl;
	            }

	        }
  	  }

  void notify(std::string message) {
    if (!getenv("SERVER") || !getenv("AUTH_TOKEN")) {
      return;
    }

    std::time_t now = std::time(NULL);
    char mbstr[sizeof "2011-10-08T07:07:09Z"];
    std::strftime(mbstr, sizeof(mbstr), "%FT%TZ", std::localtime(&now));

    std::stringstream payload;
    payload << "{\"state\":";
    payload << "\"" << message << " " << mbstr << "\"}";

    RestClient::headermap headers;
    headers["X-Auth-Token"] = getenv("AUTH_TOKEN");

    RestClient::response r = RestClient::put(getenv("SERVER"), "text/json", payload.str(), headers);
    std::cout << "Datastore called. Result:" << r.code << std::endl;
    std::cout << r.body << std::endl;
  }

  void check_object_detected(){

      bool isDetected = dInterupter->objectDetected();
      if (isDetected){
        std::cout << "Object detected" << std::endl;
      }
      else{
        std::cout << "Area is clear" << std::endl;
      }
      sleep(1);
    }


};

// Function called by worker thread for device communication
void runner(Devices& devices) {
  for (;;) {
	  devices.gpsInfo();
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

  // start worker thread for device communication
  //std::thread t1(runner, std::ref(devices));

  // wait forever for the thread to exit
  //t1.join();

  return MRAA_SUCCESS;
}
