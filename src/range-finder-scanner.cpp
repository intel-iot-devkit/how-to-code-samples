#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sstream>
#include <thread>
#include <ctime>
#include <array>
#include <string>

#include <uln200xa.h>
#include <rfr359f.h>

#include "../lib/crow/crow_all.h"
#include "html.h"

bool degrees[360];

// The hardware devices that the example is going to connect to
struct Devices
{
  upm::RFR359F* interuptor;
  upm::ULN200XA* stepper;


  Devices(){
  };

  // Initialization function
  void init() {
    // rotary connected to d2
    interuptor = new upm::RFR359F(2);

    // stepper motor connected to d9,10,11,12
    stepper = new upm::ULN200XA(4096, 9, 10, 11, 12);

    for (int i = 0; i < 360; i++){
      degrees[i] = false;
    }
  };

  // Cleanup on exit
  void cleanup() {
    delete interuptor;
    stepper->release();
    delete stepper;
  }

  void move(){
    //stepper rotates clockwise one degree at a time
    stepper->setDirection(upm::ULN200XA::DIR_CW);
    stepper->setSpeed(5);
    stepper->stepperSteps(4096/360);
  }

  void objCheck(){
    for(int i = 0; i<360; i++){
      bool isDetected = interuptor->objectDetected();
      if (isDetected){
        std::cout << "Object detected" << std::endl;
        degrees[i] = true;
      }
      else{
        std::cout << "Area is clear" << std::endl;
        degrees[i] = false;
      }
      move();
      sleep(1);
    }
  }
};

// Function called by worker thread for device communication
void runner(Devices& devices) {
  for (;;) {
    devices.objCheck();
   // sleep(1);
  }
}

Devices devices;
//currentState state;
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
  std::thread t1(runner, std::ref(devices));

  // define web server & routes
  crow::SimpleApp app;

  CROW_ROUTE(app, "/")
  ([]() {
    std::stringstream text;
    text << index_html;
    return text.str();
  });

  CROW_ROUTE(app, "/data.json")
  ([](const crow::request& req) {
    crow::json::wvalue result;
    for (int i = 0; i < 360; i++) {
      result[i] = degrees[i];
    }

    return crow::response{result};
  });

  // start web server
  app.port(3000).multithreaded().run();

  // wait forever for the thread to exit
  t1.join();

  return MRAA_SUCCESS;
}
