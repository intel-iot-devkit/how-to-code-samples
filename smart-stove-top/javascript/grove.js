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
var temp = new (require("jsupm_otp538u").OTP538U)(0, 1),
    flame = new (require("jsupm_yg1006").YG1006)(4),
    speaker = new (require("jsupm_grovespeaker").GroveSpeaker)(5);


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
