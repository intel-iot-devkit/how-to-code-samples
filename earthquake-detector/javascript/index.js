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

// Initialize the hardware for whichever kit we are using
var board;
if (config.kit) {
  board = require("./" + config.kit + ".js");
} else {
  board = require('./grove.js');
}

// The program is using the `superagent` module
// to make the remote calls to the data store
var request = require("superagent");

// Display message on RGB LCD when program checking
// USGS earthquake data
function checking() {
  console.log("Checking...");
  board.message("Checking...");
}

// Display message on RGB LCD when USGS indicated an
// earthquake occurred
function warn() {
  console.log("Earthquake!");
  board.color("red");
  board.message("Earthquake!");
}

// Display message on RGB LCD when USGS indicated no
// earthquake occurred
function noquake() {
  console.log("No quake.");
  board.color("white");
  board.message("No quake.");
}

// Calls USGS to verify that an earthquake
// has actually occurred in the user's area
function verify() {
  checking();
  function callback(err, res) {
    if (err) { return console.error("err:", err); }

    // check if there were any quakes reported
    if (res.body.features.length) {
      warn();
    } else {
      noquake();
    }

    // turn off after 15 seconds
    setTimeout(board.stop, 15000);
  }

  // we'll check for quakes in the last ten minutes
  var time = new Date();
  time.setMinutes(time.getMinutes() - 10);

  request
    .get("http://earthquake.usgs.gov/fdsnws/event/1/query")
    .query({ format: "geojson" })
    .query({ starttime: time.toISOString() })
    .query({ latitude: config.LATITUDE })
    .query({ longitude: config.LONGITUDE })
    .query({ maxradiuskm: 500 })
    .end(callback);
}

// Main function checks every
// 100ms to see if there has been motion detected
// by the accelerometer. If so, it calls verify to
// check the USGS API and see if an earthquake has
// actually occurred, and displays info on the display
function main() {

  var prev = false;

  setInterval(function() {
    var quake = board.getAcceleration();

    if (quake && !prev) { verify(); }
    prev = quake;
  }, 100);
}

main();
