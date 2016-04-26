/*
* Copyright (c) 2015-2016 Intel Corporation.
*
* Permission is hereby granted, free of charge, to any person ("User") obtaining
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

// Initialize the hardware devices
var touch = new (require("jsupm_ttp223").TTP223)(4),
    buzzer = new (require("jsupm_buzzer").Buzzer)(5),
    screen = new (require("jsupm_i2clcd").Jhd1313m1)(6, 0x3E, 0x62);

var datastore = require("./datastore");
var mqtt = require("./mqtt");

// Colors used for the RGB LED
var colors = { green: [0, 255, 0], white: [255, 255, 255] };

// Writes a message to the I2C LCD display
function message(string, color) {
  // pad string to avoid display issues
  while (string.length < 16) { string += " "; }

  screen.setCursor(0, 0);
  screen.write(string);
  screen.setColor.apply(screen, color || colors.white);
}

// Reset the state of the doorbot
function reset() {
  message("doorbot ready");
  buzzer.setVolume(0.5);

  // we call #stopSound twice because otherwise buzzer may not actually stop
  buzzer.stopSound();
  buzzer.stopSound();
}

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
  message("ding dong!", colors.green);
  buzzer.playSound(2600, 0);
}

// Main function, resets the doorbot, and then checks every
// 50ms to see if anyone has rung the bell
function main() {
  reset();

  var prev = false;

  setInterval(function() {
    var current = touch.isPressed();
    if (current && !prev) { dingdong(); }
    if (!current && prev) { reset(); }
    prev = current;
  }, 50);
}

main();
