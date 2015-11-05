#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <sstream>

#include <ttp223.h>
#include <buzzer.h>
#include <jhd1313m1.h>

#include "../lib/restclient-cpp/include/restclient-cpp/restclient.h"

// call data server to increment the count of visitors who rang the doorbell
void increment() {
  if (!getenv("SERVER") || !getenv("AUTH_TOKEN")) {
    std::cerr << "Server not configured." << std::endl;
    return;
  }

  RestClient::headermap headers;
  headers["X-Auth-Token"] = getenv("AUTH_TOKEN");

  RestClient::response r = RestClient::get(getenv("SERVER"), headers);
  std::cerr << r.code << std::endl;
  std::cerr << r.body << std::endl;
}

struct Devices
{
  upm::TTP223* touch;
  upm::Buzzer* buzzer;
  upm::Jhd1313m1* screen;

  Devices(){
  };

  void init() {
    // touch sensor connected to D4 (digital in)
    touch = new upm::TTP223(4);

    // buzzer connected to D5 (digital out)
    buzzer = new upm::Buzzer(5);
    stop_ringing();

    // screen connected to the default I2C bus
    screen = new upm::Jhd1313m1(0);
  };

  void cleanup() {
    delete touch;
    delete buzzer;
    delete screen;
  }

  void reset() {
    message("doorbot ready");
    stop_ringing();
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

  void dingdong() {
    increment();
    message("ding dong!");
    buzzer->playSound(266, 0);
  }

  void stop_ringing() {
    buzzer->stopSound();
    buzzer->stopSound();
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

  bool wasPressed = false;
  bool currentlyPressed = false;

  for (;;) {
    currentlyPressed = devices.touch->isPressed();
    if ( currentlyPressed && ! wasPressed ) {
      devices.dingdong();
    } else if (! currentlyPressed && wasPressed ) {
      devices.reset();
    }

    wasPressed = currentlyPressed;
    sleep(1);
  }

  return MRAA_SUCCESS;
}
