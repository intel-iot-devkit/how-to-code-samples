#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <ctime>
#include <chrono>
#include <signal.h>

using namespace std;

#include <ssd1308.h>
#define OLED_DEVICE_ADDRESS    0x3C
#define OLED_BUS_NUMBER        0x0

#define GRACE_PERIOD 15

#include "../lib/restclient-cpp/include/restclient-cpp/restclient.h"

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

// Log the event to the remote datastore
void log(string event) {
  cout << event << endl;

  if (!getenv("SERVER") || !getenv("AUTH_TOKEN")) {
    cerr << "Datastore not configured." << endl;
    return;
  }

  time_t now = time(NULL);
  char mbstr[sizeof "2011-10-08T07:07:09Z"];
  strftime(mbstr, sizeof(mbstr), "%FT%TZ", localtime(&now));

  stringstream payload;
  payload << "{\"value\":";
  payload << "\"" << event << mbstr << "\"}";

  RestClient::headermap headers;
  headers["X-Auth-Token"] = getenv("AUTH_TOKEN");

  RestClient::response r = RestClient::put(getenv("SERVER"), "text/json", payload.str(), headers);

  cout << "Datastore updated." << endl;
}

struct BLE
{
  int hciSocket;
  struct hci_filter newFilter, originalFilter;
  map<string, time_t> found;

  void open() {
    hciSocket = hci_open_dev(0);

    // save original filter
    socklen_t originalFilterLen = sizeof(originalFilter);
    getsockopt(hciSocket, SOL_HCI, HCI_FILTER, &originalFilter, &originalFilterLen);

    // setup new filter
    hci_filter_clear(&newFilter);
    hci_filter_set_ptype(HCI_EVENT_PKT, &newFilter);
    hci_filter_set_event(EVT_LE_META_EVENT, &newFilter);
    setsockopt(hciSocket, SOL_HCI, HCI_FILTER, &newFilter, sizeof(newFilter));

    // disable scanning just in case scanning was already happening,
    // otherwise hci_le_set_scan_parameters call will fail
    hci_le_set_scan_enable(hciSocket, 0x00, 0, 1000);

    // set scan params
    hci_le_set_scan_parameters(hciSocket, 0x01, htobs(0x0010), htobs(0x0010), 0x00, 0, 1000);

    // start scanning
    hci_le_set_scan_enable(hciSocket, 0x00, 0, 1000);
    hci_le_set_scan_enable(hciSocket, 0x01, 0, 1000);

    printf("Scanning ...\n");
  }

  void close() {
    // put back original filter
    setsockopt(hciSocket, SOL_HCI, HCI_FILTER, &originalFilter, sizeof(originalFilter));

    // stop scanning
    hci_le_set_scan_enable(hciSocket, 0x00, 0, 1000);

    // close BLE adaptor
    hci_close_dev(hciSocket);
  }

  string get_device()
  {
    unsigned char buf[HCI_MAX_EVENT_SIZE], *ptr;
    int len;

    evt_le_meta_event *meta;
    le_advertising_info *info;
    char addr[18];

    fd_set rfds;
    struct timeval tv;
    int selectRetval;

    FD_ZERO(&rfds);
    FD_SET(hciSocket, &rfds);

    tv.tv_sec = 1;
    tv.tv_usec = 0;

    // wait to see if we have data on the socket
    selectRetval = select(hciSocket + 1, &rfds, NULL, NULL, &tv);

    if (-1 == selectRetval) {
      return "error";
    }

    len = read(hciSocket, buf, sizeof(buf));
    if (len == 0) {
    	return "";
    }

    ptr = buf + (1 + HCI_EVENT_HDR_SIZE);
    len -= (1 + HCI_EVENT_HDR_SIZE);

    meta = (evt_le_meta_event *) ptr;

    if (meta->subevent != 0x02)
      return "error";

    info = (le_advertising_info *) (meta->data + 1);
    ba2str(&info->bdaddr, addr);
    discovered(addr);
    return addr;
  }

  // Check to see if any devices that we could previously
  // detect, are no longer within BLE radio range
  void check_for_missing() {
    chrono::time_point<chrono::system_clock> current;
    current = chrono::system_clock::now();

    for (map<string, time_t>::iterator it = found.begin(); it != found.end(); ++it) {
      chrono::duration<double> elapsed = current - chrono::system_clock::from_time_t(it->second);
      if (elapsed.count() > GRACE_PERIOD) {
        string val = "Exiting " + it->first;
        log(val);
        found.erase(it);
      }
    }
  }

  // when we find a device, we add to the list
  void discovered(string address) {
    time_t now = time(NULL);
    if (found.find(address) == found.end())
      log("Entered " + address);

    found[address] = now;
  }
};

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
  void message(const string& input) {
    string text(input);
    text.resize(16, ' ');

    screen->clear();
    screen->home();
    screen->write(text);
  }
};

Devices devices;
BLE ble;

// Exit handler for program
void exit_handler(int param)
{
  ble.close();
  devices.cleanup();
  exit(1);
}

// The main function for the example program
int main()
{
  string address;

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
  ble.open();

  devices.message("Scanning...");

  chrono::time_point<chrono::system_clock> last, current;
  last = chrono::system_clock::now();

  for (;;) {
    current = chrono::system_clock::now();
    address = ble.get_device();
    devices.message(address);

    chrono::duration<double> elapsed = current - last;
    if (elapsed.count() > GRACE_PERIOD / 4) {
      ble.check_for_missing();
      last = current;
    }
  }

  return MRAA_SUCCESS;
}
