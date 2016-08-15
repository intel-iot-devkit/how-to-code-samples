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

var mraa = require('mraa');

// devices
var touch, buzzer, screen;

// pins
var touchPin = 4,
    buzzerPin = 5,
    i2cBus = 6;

// Initialize the Grove hardware devices
exports.init = function(config) {
  if (config.platform == "firmata") {
    // open connection to firmata
    mraa.addSubplatform(mraa.GENERIC_FIRMATA, "/dev/ttyACM0");

    touchPin += 512;
    buzzerPin += 512;
    i2cBus = 512;
  }

  touch = new (require("jsupm_ttp223").TTP223)(touchPin);
  buzzer = new (require("jsupm_buzzer").Buzzer)(buzzerPin);
  screen = new (require("jsupm_i2clcd").Jhd1313m1)(i2cBus, 0x3E, 0x62);
}

// Colors used for the RGB LED
var colors = { green: [0, 255, 0], white: [255, 255, 255] };

// Writes a message to the I2C LCD display
exports.message = function(string, color) {
  // pad string to avoid display issues
  while (string.length < 16) { string += " "; }

  screen.setCursor(0, 0);
  screen.write(string);
  screen.setColor.apply(screen, colors[color] || colors.white);
}

// Reset the state of the doorbot
exports.reset = function() {
  this.message("doorbot ready");
  buzzer.setVolume(0.5);

  // we call #stopSound twice because otherwise buzzer may not actually stop
  buzzer.stopSound();
  buzzer.stopSound();
}

// play a sound
exports.playSound = function(hz, len) {
  buzzer.playSound(hz, len);
}

// is the touch sensor pressed?
exports.touchPressed = function() {
  return touch.isPressed();
}
