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

// Initialize the Grove hardware devices
var accelerometer = require("jsupm_mma7660");
var accel = new accelerometer.MMA7660(0, 76),
    screen = new (require("jsupm_i2clcd").Jhd1313m1)(6, 0x3E, 0x62);

// Initialize the accelerometer to enable 64 samples per second
accel.setModeStandby();
accel.setSampleRate(1);
accel.setModeActive();

var ax, ay, az;
ax = accelerometer.new_floatp();
ay = accelerometer.new_floatp();
az = accelerometer.new_floatp();

// Colors used for the RGB LED
var colors = { red: [255, 0, 0], white: [255, 255, 255] };

// Sets the background color on the RGB LED
exports.color = function(string) {
  screen.setColor.apply(screen, colors[string] || colors.white);
}

// Displays a message on the RGB LED
exports.message = function(string, line) {
  // pad string to avoid display issues
  while (string.length < 16) { string += " "; }

  screen.setCursor(line || 0, 0);
  screen.write(string);
}

exports.getAcceleration = function() {
  accel.getAcceleration(ax, ay, az);
  return { x: accelerometer.floatp_value(ax),
           y: accelerometer.floatp_value(ay),
           z: accelerometer.floatp_value(az) }
}
