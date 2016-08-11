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
var mic = require("jsupm_mic");

// devices
var sound, vibration, screen, ctx;

// pins
var soundPin = 0,
    vibrationPin = 2,
    i2cBus = 6;

// Initialize the Grove hardware devices
exports.init = function(config) {
  if (config.platform == "firmata") {
    // open connection to firmata
    mraa.addSubplatform(mraa.GENERIC_FIRMATA, "/dev/ttyACM0");

    soundPin += 512;
    vibrationPin += 512;
    i2cBus = 512;
  }

  sound = new mic.Microphone(soundPin),
  vibration = new (require("jsupm_ldt0028").LDT0028)(vibrationPin),
  screen = new (require("jsupm_i2clcd").Jhd1313m1)(i2cBus, 0x3E, 0x62);

  ctx = new mic.thresholdContext();
  ctx.averageReading = 0;
  ctx.runningAverage = 0;
  ctx.averagedOver = 2;
}

// Display a warning message on the I2C LCD display
exports.warn = function() {
  screen.setCursor(0, 0);
  screen.write("EQUIPMENT IN USE");
  screen.setColor(255, 255, 255);
}

// Clears the I2C LCD display
exports.clear = function() {
  screen.setCursor(0, 0);
  screen.write("                ");
  screen.setColor(0, 0, 0);
}

// reads vibration sensor
exports.getVibration = function(t) {
  var sample = vibration.getSample();
  return (sample >= t);
}

// reads audio level from mic
exports.getNoise = function(threshold) {
  var buffer = new mic.uint16Array(128),
      len = sound.getSampledWindow(2, 128, buffer);

  if (!len) { return; }

  var noise = sound.findThreshold(ctx, threshold, buffer, len);
  return noise;
}
