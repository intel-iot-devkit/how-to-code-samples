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

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

struct BLE
{
  int deviceHandle;
  struct hci_filter newFilter, originalFilter;

  void open() {
    deviceHandle = hci_open_dev(0);

    // save original filter
    socklen_t originalFilterLen = sizeof(originalFilter);
    getsockopt(deviceHandle, SOL_HCI, HCI_FILTER, &originalFilter, &originalFilterLen);

    // setup new filter
    hci_filter_clear(&newFilter);
    hci_filter_set_ptype(HCI_EVENT_PKT, &newFilter);
    hci_filter_set_event(EVT_LE_META_EVENT, &newFilter);
    setsockopt(deviceHandle, SOL_HCI, HCI_FILTER, &newFilter, sizeof(newFilter));

    // disable scanning just in case scanning was already happening,
    // otherwise hci_le_set_scan_parameters call will fail
    hci_le_set_scan_enable(deviceHandle, 0x00, 0, 1000);

    // set scan params
    hci_le_set_scan_parameters(deviceHandle, 0x01, htobs(0x0010), htobs(0x0010), 0x00, 0x00, 1000);

    // start scanning
    hci_le_set_scan_enable(deviceHandle, 0x00, 1, 1000);
    hci_le_set_scan_enable(deviceHandle, 0x01, 1, 1000);

    printf("Scanning ...\n");
  }

  void close() {
    // put back original filter
    setsockopt(deviceHandle, SOL_HCI, HCI_FILTER, &originalFilter, sizeof(originalFilter));

    // stop scanning
    hci_le_set_scan_enable(deviceHandle, 0x00, 1, 1000);

    // close BLE adaptor
    hci_close_dev(deviceHandle);
  }

  std::string get_device()
  {
    unsigned char buf[HCI_MAX_EVENT_SIZE], *ptr;
    int len;

    evt_le_meta_event *meta;
    le_advertising_info *info;
    char addr[18];

    while ((len = read(deviceHandle, buf, sizeof(buf))) < 0) {
      if (errno == EAGAIN || errno == EINTR)
        continue;
    }

    ptr = buf + (1 + HCI_EVENT_HDR_SIZE);
    len -= (1 + HCI_EVENT_HDR_SIZE);

    meta = (evt_le_meta_event *) ptr;

    if (meta->subevent != 0x02)
      return "error";

    info = (le_advertising_info *) (meta->data + 1);
    ba2str(&info->bdaddr, addr);
    return addr;
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
  void message(const std::string& input) {
    screen->clear();
    screen->write(input);
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
  std::string address;

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
  ble.open();

  for (;;) {
    address = ble.get_device();
    std::cout << address << std::endl;
    //devices.message(address);
  }

  return MRAA_SUCCESS;
}
