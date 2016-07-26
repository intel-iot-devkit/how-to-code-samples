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

var datastore = require("./datastore");
var mqtt = require("./mqtt");

// Display and then store record in the remote datastore/mqtt server
// of how long the equipment being monitored was in use for
function notify(state) {
  console.log("Value: " + state + " " + new Date().toISOString());
  var payload = { value: state + " " + new Date().toISOString() };

  datastore.log(config, payload);
  mqtt.log(config, payload);
}

// Main function, checks every 20ms to see if either vibrations or
// sounds are detected.
// If either are detected, and the program is not already in the
// process of notifying about the activity, it display a message
// and notifies the server.
function main() {
  var tripped = false;
  console.log("config.VIBRATION_THRESHOLD: ", config.VIBRATION_THRESHOLD);

  setInterval(function() {
    var movement = board.getVibration(config.VIBRATION_THRESHOLD);
    console.log("Movement: ", movement);

    var noise = board.getNoise(config.NOISE_THRESHOLD);
    console.log("Noise: ", noise);

    if (movement && noise && !tripped) {
      notify("start");
      board.warn();
    }

    if (!(movement && noise) && tripped) {
      notify("stop");
      board.clear();
    }

    tripped = movement && noise;
  }, 20);
}

main();
