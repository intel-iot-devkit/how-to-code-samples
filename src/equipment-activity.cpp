#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <ctime>

#include <mic.h>
#include <ldt0028.h>
#include <jhd1313m1.h>

#include "../lib/restclient-cpp/include/restclient-cpp/restclient.h"

const int VIBRATION_THRESHOLD = 500;
const int NOISE_THRESHOLD = 140;

// Call data server to log activity to server
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

// TODO
struct Devices
{
  upm::Microphone *mic = NULL;
  thresholdContext micCtx;
  uint16_t soundBuffer [128];

  upm::LDT0028* vibe;
  upm::Jhd1313m1* screen;

  Devices(){
  };

  // TODO
  void init() {
    // mic connected to A0 (analog in)
    mic = new upm::Microphone(0);
    micCtx.averageReading = 0;
    micCtx.runningAverage = 0;
    micCtx.averagedOver   = 2;

    // vibration sensor connected to A2 (analog in)
    vibe = new upm::LDT0028(2);

    // screen connected to the default I2C bus
    screen = new upm::Jhd1313m1(0);
  };

  // TODO
  void cleanup() {
    delete mic;
    delete vibe;
    delete screen;
  }

  // TODO
  void reset() {
    message("ready");
  }

  // TODO
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

  // TODO
  bool is_movement() {
    return (vibe->getSample() >= VIBRATION_THRESHOLD);
  }

  // TODO
  bool is_noise() {
    int len = mic->getSampledWindow(2, 128, soundBuffer);
    if (len) {
      int thresh = mic->findThreshold(&micCtx, 30, soundBuffer, len);
        if (thresh) {
          return true;
        }
    }

    return false;
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
  devices.reset();

  bool movement = false;
  bool noise = false;
  bool inUse = false;

  for (;;) {
    movement = devices.is_movement();
    noise = devices.is_noise();
    if ( movement && noise && !inUse ) {
      notify("start");
      devices.message("start");
    } else if ( !(movement && noise) && inUse ) {
      notify("stop");
      devices.message("stop");
    }

    inUse = movement && noise;
    sleep(1);
  }

  return MRAA_SUCCESS;
}
