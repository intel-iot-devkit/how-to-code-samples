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

// Initialize the hardware devices
var moisture = new (require("jsupm_grovemoisture").GroveMoisture)(0),
    speaker = new (require("jsupm_grovespeaker").GroveSpeaker)(5);

var datastore = require("./datastore");
var mqtt = require("./mqtt");

// Store record in the remote datastore when moisture is detected
function notify() {
  console.log("Moisture alarm");

  var payload = { value: new Date().toISOString() };

  datastore.log(config, payload);
  mqtt.log(config, payload);
}

// Alert when moisture is detected
function alert() {
  notify();

  // play quick chime
  speaker.playSound("a", true, "low");
  speaker.playSound("c", true, "low");
  speaker.playSound("g", true, "low");
}

// The main function monitors the connected hardware every 1
// second to check if moisture has been detected,
// indicating a possible flood.
// If so, it calls the `alert()` function.
function main() {
  var prev;

  setInterval(function() {
    var value = moisture.value();
    if (prev === 0 && value !== 0) { alert(); }
    prev = value;
  }, 1000);
}

main();
