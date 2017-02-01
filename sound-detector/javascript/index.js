/*
* Copyright (c) 2015 - 2017 Intel Corporation.
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

// Store record in the remote mqtt or datastore for the sound detector
function log(val) {
  var payload = { value: val };
  datastore.log(config, payload);
  mqtt.log(config, payload);
}

// Display value on the LCD for the microphone
function display(val) {
  console.log("Sound:", val);
  board.message("Sound " + val.toString());
  if (val < 1) {
    board.color("white");
  } else if (val < 2) {
    board.color("yellow");
  } else if (val < 3) {
    board.color("green");
  } else if (val < 4) {
    board.color("blue");
  } else if (val < 5) {
    board.color("purple");
  } else {
    board.color("red");
  }
}

// Main function checks and displays the mic level every 100 ms,
// it also calls the `log()` function every 5 seconds.s
function main() {
  board.clear();

  var level;
  setInterval(function() {
    level = board.getMicLevel();
    display(level);
  }, 100);

  setInterval(function() {
    log(level);
  }, 5000);
}

main();
