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

var datastore = require("./datastore");
const services = require("./services");

var LOCATION;

// Initialize the hardware for whichever kit we are using
var board;
if (config.kit) {
  board = require("./" + config.kit + ".js");
} else {
  board = require('./grove.js');
}

// Store record in the remote datastore when close call
// event has occurred
function report() {
  console.log("close call at", LOCATION);
  var payload = { value: new Date().toISOString() + " " + LOCATION };

  datastore.log(config, payload);
  services.log(config, payload);
}

// The main function checks the distance sensor every 1 second,
// and if any object is detected that is too close, it stores
// that the event occured in the remote datastore
function main() {
  var prev = false;

  setInterval(function() {
    var close = board.objectDetected();

    if (board.gpsDataAvailable()) {
        LOCATION = board.getGpsLocation();
    } else {
        console.log("No GPS data available...")
    }
    if (LOCATION && close && !prev) { report(); }

    prev = close;
  }, 1000);
}

main();
