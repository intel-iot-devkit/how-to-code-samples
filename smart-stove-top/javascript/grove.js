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
var temp, flame, speaker;

// pins
var ambientTempPin = 0,
    objectTempPin = 1,
    flamePin = 4,
    speakerPin = 5,
    referenceVoltage = 5.0;

// Initialize the Grove hardware devices
exports.init = function(config) {
  if (config.platform == "firmata") {
    // open connection to firmata
    mraa.addSubplatform(mraa.GENERIC_FIRMATA, "/dev/ttyACM0");

    ambientTempPin += 512;
    objectTempPin += 512;
    flamePin += 512;
    speakerPin += 512;
    referenceVoltage = 3.3;
  }

  temp = new (require("jsupm_otp538u").OTP538U)(ambientTempPin, objectTempPin, referenceVoltage),
  flame = new (require("jsupm_yg1006").YG1006)(flamePin),
  speaker = new (require("jsupm_grovespeaker").GroveSpeaker)(speakerPin);
}

// Plays an audible alarm when the temperature has exceeded
// the target temperature
exports.tempAlarm = function() {
  speaker.playSound("a", true, "low");
  speaker.playSound("c", true, "low");
  speaker.playSound("d", true, "low");
  speaker.playSound("b", false, "low");
}

// Plays an audible alarm when the flame sensor indicates
// a possible fire
exports.fireAlarm = function() {
  var i = 0;
  while (i < 10) {
    speaker.playSound("a", true, "high");
    speaker.playSound("c", true, "high");
    speaker.playSound("g", true, "med");
    i++;
  }
}

// returns the temperature of the pot or pan on the stovetop
// using the IR sensor
exports.objectTemperature = function() {
  return temp.objectTemperature();
}

// returns if an open flame is detected using flame sensor
exports.flameDetected = function() {
  return flame.flameDetected();
}
