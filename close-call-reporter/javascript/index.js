/*
* Copyright (c) 2015-2016 Intel Corporation.
*
* Permission is hereby granted, free of charge, to any person (“User”) obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* User understands, acknowledges, and agrees that: (i) the Software is sample software;
* (ii) the Software is not designed or intended for use in any medical, life-saving
* or life-sustaining systems, transportation systems, nuclear systems, or for any
* other mission-critical application in which the failure of the system could lead to
* critical injury or death; (iii) the Software may not be fully tested and may contain
* bugs or errors; (iv) the Software is not intended or suitable for commercial release;
* (v) no regulatory approvals for the Software have been obtained, and therefore Software
* may not be certified for use in certain countries or environments.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
* LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
* OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
* WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

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

var datastore = require("./datastore");
var mqtt = require("./mqtt");

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
  var payload = { value: new Date().toISOString() + " " + LOCATION };

  datastore.log(config, payload);
  mqtt.log(config, payload);
}

// The main function checks the distance sensor every 1 second,
// and if any object is detected that is too close, it stores
// that the event occured in the remote datastore
function main() {
  var prev = false;

  setInterval(function() {
    var close = dist.objectDetected();

    if (gps.dataAvailable()) {
        getLocation();
    } else {
        console.log("No GPS data available...")
    }
    if (LOCATION && close && !prev) { report(); }

    prev = close;
  }, 1000);
}

main();
