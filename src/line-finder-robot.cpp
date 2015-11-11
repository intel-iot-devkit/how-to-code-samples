#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sstream>
#include <thread>
#include <ctime>

#include <grovelinefinder.h>
#include <uln200xa.h>

#include "../lib/restclient-cpp/include/restclient-cpp/restclient.h"
#include "../lib/crow/crow_all.h"

using namespace std;

//function to send data to server
void notify(std::string message) {
  if (!getenv("SERVER") || !getenv("AUTH_TOKEN")) {
    std::cerr << "Server not configured." << std::endl;
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

  std::cerr << r.code << std::endl;
  std::cerr << r.body << std::endl;
}

// The hardware devices that the example is going to connect to
struct Devices
{
  upm::GroveLineFinder* finder;
  upm::ULN200XA* stepLeft;
  upm::ULN200XA* stepRight;

  Devices(){
  };

  // Initialization function
  void init() {
    // rotary connected to A0 (analog in)
    finder = new upm::GroveLineFinder(2);
    // left stepper motor connected to d9,10,11,12
    stepLeft = new upm::ULN200XA(4096, 9, 10, 11, 12);
    // right stepper motor connected to 4, 5, 6, 7
    stepRight = new upm::ULN200XA(4096, 4, 5, 6, 7);

  };

  // Cleanup on exit
  void cleanup() {
    delete finder;
    stepLeft->release();
    stepRight->release();
    delete stepLeft;
    delete stepRight;


  }
  // function to make robot find and follow a black line
  void findFollowLine(){
	  for (;;){
		  bool val = finder->blackDetected();

		  if (val){
			  moveForward();
			  notify("Moving on line. \n");
		  }
		  else
			  pivotClockwise();
	  }
  }
  //function to have both  motors move forward
  //assuming motors are on opposite sides of each other
  void moveForward(){
	  stepLeft->setSpeed(5);
	  stepRight->setSpeed(5);

	  stepLeft->setDirection(upm::ULN200XA::DIR_CW);
	  stepRight->setDirection(upm::ULN200XA::DIR_CCW);

	  stepLeft->stepperSteps(4096);
	  stepRight->stepperSteps(4096);

  }
  //function to have both motors pivot clockwise
  //assuming motors are on opposite sides of each other
  void pivotClockwise(){
	  stepLeft->setSpeed(5);
	  stepRight->setSpeed(5);

	  stepLeft->setDirection(upm::ULN200XA::DIR_CW);
	  stepRight->setDirection(upm::ULN200XA::DIR_CW);

	  stepLeft->stepperSteps(4096);
	  stepRight->stepperSteps(4096);

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

  for(;;){
	  devices.findFollowLine();
  }

  return MRAA_SUCCESS;
}
