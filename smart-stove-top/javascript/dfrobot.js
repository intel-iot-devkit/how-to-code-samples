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

// Initialize the DFRobot hardware devices
var temp = new (require("jsupm_grove").GroveTemp)(1), // A1
    flame = new (require("jsupm_grove").GroveTemp)(2), // A2
    buzzer = new (require("jsupm_buzzer").Buzzer)(3), // A3

// Plays an audible alarm when the temperature has exceeded
// the target temperature
exports.tempAlarm = function() {
  buzzer.setVolume(0.5);
  buzzer.playSound(2600, 1000);
}

// Plays an audible alarm when the flame sensor indicates
// a possible fire
exports.fireAlarm = function() {
  var i = 0;
  while (i < 10) {
    buzzer.playSound(2600, 500);
    buzzer.playSound(3600, 500);
    buzzer.playSound(2600, 500);
    i++;
  }
}

// returns the temperature of the pot or pan on the stovetop
// using the analog temperature sensor
exports.objectTemperature = function() {
  return temp.objectTemperature();
}

// returns if an open flame is detected using flame sensor
exports.flameDetected = function() {
  return (flame.value() >= 100);
}
