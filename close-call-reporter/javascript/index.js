"use strict";

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

// The program is using the `superagent` module
// to make the remote calls to the data store
var request = require("superagent");

// The program is using the `nmea` module
// parse the data coming from the GPS
var nmea = require("nmea");

// Initialize the hardware devices
var GPS = require("jsupm_ublox6");
var gps = new GPS.Ublox6(0),
    dist = new (require("jsupm_rfr359f").RFR359F)(2);

if (!gps.setupTty(GPS.int_B9600)) {
  throw new Error("Failed to setup tty port parameters");
}

var GPS_BUFFER_LENGTH = 256,
    GPS_BUFFER = new GPS.charArray(GPS_BUFFER_LENGTH),
    LOCATION;

// Reads the GPS to get the current location
function getLocation() {
  var data = "";

  // batch data so we get complete NMEA sentences
  while (data.slice(-1) !== "\n") {
    var length = gps.readData(GPS_BUFFER, GPS_BUFFER_LENGTH);

    if (!length) {
      // some sort of read error occured
      throw new Error("port read error");
    }

    // read only the number of characters
    // specified by gps.readData
    for (var i = 0; i < length; i++) {
      data += GPS_BUFFER.getitem(i);
    }
  }

  data.split("\n").forEach(function(str) {
    try {
      if (nmea.parse(str).sentence === "GGA") { LOCATION = str; }
    } catch (e) {
      // issue parsing NMEA data
    }
  });
}

// Store record in the remote datastore when close call
// event has occurred
function report() {
  console.log("close call at", LOCATION);
  if (!config.SERVER || !config.AUTH_TOKEN) {
    return;
  }

  function callback(err, res) {
    if (err) { return console.error("err:", err); }
    console.log("close call reported");
  }

  request
    .put(config.SERVER)
    .set("X-Auth-Token", config.AUTH_TOKEN)
    .send({ value: new Date().toISOString() + " " + LOCATION })
    .end(callback);
}

// The main function checks the distance sensor every 100ms,
// and if any object is detected that is too close, it stores
// that the event occured in the remote datastore
function main() {
  var prev = false;

  setInterval(function() {
    var close = dist.objectDetected();

    if (gps.dataAvailable()) { getLocation(); }
    if (LOCATION && close && !prev) { report(); }

    prev = close;
  }, 100);
}

main();
