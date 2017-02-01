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

var exports = module.exports = {};

var mraa = require("mraa");
var mic = require("jsupm_mic");

// devices
var sound, ctx, screen;

// pins
var soundPin = 2,
    i2cBus = 6;

var colors = { red: [255, 0, 0], purple: [255, 0, 255], blue: [0, 0, 255], green: [0, 128, 0], yellow: [255, 255, 0], white: [255, 255, 255] };

// Initialize the Grove hardware devices
exports.init = function(config) {
  if (config.platform == "firmata") {
    // open connection to firmata
    mraa.addSubplatform(mraa.GENERIC_FIRMATA, "/dev/ttyACM0");

    soundPin += 512;
    i2cBus = 512;
  }

  sound = new mic.Microphone(soundPin),
  screen = new (require("jsupm_i2clcd").Jhd1313m1)(i2cBus, 0x3E, 0x62);

  ctx = new mic.thresholdContext();
  ctx.averageReading = 0;
  ctx.runningAverage = 0;
  ctx.averagedOver = 2;
}

// reads audio level from mic
exports.getMicLevel = function() {
  var buffer = new mic.uint16Array(128),
      len = sound.getSampledWindow(2, 128, buffer);

  if (!len) { return; }

  var level = sound.findThreshold(ctx, 30, buffer, len);
  return level / 100;
}

// Clears the I2C LCD display
exports.clear = function() {
  screen.setCursor(0, 0);
  screen.write("                ");
  screen.setColor(0, 0, 0);
}

// Display a message on the I2C LCD display
exports.message = function(text) {
  screen.setCursor(0, 0);
  screen.write(text);
}

// Set color on the I2C LCD display
exports.color = function(color) {
  screen.setColor.apply(screen, colors[color]);
}
