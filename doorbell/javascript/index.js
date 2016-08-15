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
board.init(config);

var datastore = require("./datastore");
var mqtt = require("./mqtt");

// Increment the datastore/mqtt server count of doorbell rings
function increment() {
  console.log("doorbell ring");

  datastore.increment(config);
  mqtt.increment(config);
}

// Logs a doorbell ring, prints a message to the display,
// and rings the buzzer
function dingdong() {
  increment();
  board.message("ding dong!", "green");
  board.playSound(2600, 0);
}

// Main function, resets the doorbot, and then checks every
// 50ms to see if anyone has rung the bell
function main() {
  board.reset();

  var prev = false;

  setInterval(function() {
    var current = board.touchPressed();
    if (current && !prev) { dingdong(); }
    if (!current && prev) { board.reset(); }
    prev = current;
  }, 50);
}

main();
