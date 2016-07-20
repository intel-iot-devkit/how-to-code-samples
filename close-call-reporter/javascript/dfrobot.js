/*
* Copyright (c) 2015 - 2016 Intel Corporation.
*
* Permission is hereby granted, free of charge, to any person obtaining
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
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
* LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
* OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
* WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

"use strict";

var exports = module.exports = {};

// The program is using the `nmea` module
// parse the data coming from the GPS
var nmea = require("nmea");

// Initialize the DFRobot hardware devices
var GPS = require("jsupm_ublox6");
var gps = new GPS.Ublox6(0),
    dist = new (require("jsupm_rfr359f").RFR359F)(2);

if (!gps.setupTty(GPS.int_B9600)) {
  throw new Error("Failed to setup tty port parameters");
}

var GPS_BUFFER_LENGTH = 256,
    GPS_BUFFER = new GPS.charArray(GPS_BUFFER_LENGTH);

// Is there info available to reads from the GPS?
exports.gpsDataAvailable = function() {
  return gps.dataAvailable();
}

// Reads the GPS to get the current location
exports.getGpsLocation = function() {
  var data = "",
      location;

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
      if (nmea.parse(str).sentence === "GGA") { location = str; }
    } catch (e) {
      // issue parsing NMEA data
    }
  });

  return location;
}

// was an object detected by the IR range finder?
exports.objectDetected = function() {
  return dist.objectDetected();
}
