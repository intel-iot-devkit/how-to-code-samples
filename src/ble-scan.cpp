#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <ctime>
#include <signal.h>

#include <ssd1308.h>
#define OLED_DEVICE_ADDRESS    0x3C
#define OLED_BUS_NUMBER        0x0

#include "../lib/restclient-cpp/include/restclient-cpp/restclient.h"

// The hardware devices that the example is going to connect to
struct Devices
{
  upm::SSD1308 *screen;

  Devices(){
  };

  // Initialization function
  void init() {
    // screen connected to the default I2C bus
    screen = new upm::SSD1308(OLED_BUS_NUMBER, OLED_DEVICE_ADDRESS);
  };

  // Cleanup on exit
  void cleanup() {
    delete screen;
  }

  // Display a message on the OLED
  void message(const std::string& input) {
    std::string text(input);
    text.resize(16, ' ');

    screen->clear();
    screen->write(text);
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
int main()
{
  // handles ctrl-c or other orderly exits
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
    sleep(1);
  }

  return MRAA_SUCCESS;
}
