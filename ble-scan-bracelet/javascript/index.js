"use strict";

// Scans for BLE peripherals, then prints message to OLED
// display when they enter/exit. Based on the example from
// https://github.com/sandeepmistry/noble/blob/master/examples/enter-exit.js

var THRESHOLD = -90,
    GRACE_PERIOD = 2000;

// The program is using the Node.js built-in `fs` module
// to load the config.json and any other files needed
var fs = require("fs");

// The program is using the Node.js built-in `path` module to find
// the file path to needed files on disk
var path = require("path");

// Load configuration data from `config.json` file. Edit this file
// to change to correct values for your configuration
var config = JSON.parse(
  fs.readFileSync(path.join(__dirname, "config.json"))
);

// The program is using the `noble` module
// to communicate with Bluetooth LE devices
var noble = require("noble");

// The program is using the `superagent` module
// to make the remote calls to the data store
var request = require("superagent");

// Initialize the hardware device
var screen = new (require("jsupm_i2clcd").SSD1308)(0, 0x3C);

var known = {};

// Displays a message on the OLED display
function message(msg) {
  screen.clear();
  screen.home();
  screen.write(msg);
}

// Logs record in the remote datastore
// of BLE devices entering or exiting BLE radio range
function log(name, state) {
  console.log(name, state);
  message(name + " " + state);

  if (!config.SERVER || !config.AUTH_TOKEN)
  {
    return;
  }

  function callback(err, res) {
    if (err) { console.error("err:", err); }
    console.log("Device logged to datastore");
  }

  request
    .put(config.SERVER)
    .set("X-Auth-Token", config.AUTH_TOKEN)
    .send({ value: name + " " + state + " " + new Date().toISOString() })
    .end(callback);
}

// Check to see if any devices that we could previously
// detect, are no longer within BLE radio range
function checkForMissing() {
  var id, p, name;

  for (id in known) {
    if (known[id].lastSeen >= Date.now() - GRACE_PERIOD) { return; }

    p = known[id];
    if ( p.advertisement) {
      name = p.advertisement.localName;
      log(name, "exited");
    }

    delete known[id];
  }
}

// Called whenever a new BLE device is discovered
function discovered(p) {
  if (p.rssi < THRESHOLD) { return; }

  var id = p.id,
      name = p.advertisement.localName;

  // check if new peripheral
  if (!known[id]) {
    log(name, "entered");
    known[id] = { peripheral: p };
  }

  known[id].lastSeen = Date.now();
}

// Check for devices that are no longer able to be detected
// every 500 ms
setInterval(checkForMissing, GRACE_PERIOD / 4);

// Call back when new BLE devices are discovered
noble.on("discover", discovered);

// Once the BLE adapter is powered on, start scanning
noble.on("stateChange", function(state) {
  if (state === "poweredOn") { return noble.startScanning([], true); }
  noble.stopScanning();
});
