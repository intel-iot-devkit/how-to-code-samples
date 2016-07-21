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
    buzzer = new (require("jsupm_buzzer").Buzzer)(2), // A2
    screen = new (require("jsupm_i2clcd").SAINSMARTKS)(8, 9, 4, 5, 6, 7, 0);

// No color support on this LCD display
exports.color = function(string) {
  return;
}

// Displays a message on the LCD
exports.message = function(string, line) {
  // pad string to avoid display issues
  while (string.length < 16) { string += " "; }

  screen.setCursor(line || 0, 0);
  screen.write(string);
}

// Sound an audible alarm when it is time to get up
exports.buzz = function() {
  buzzer.setVolume(0.5);
  buzzer.playSound(2600, 0);
}

// Turn off the audible alarm
exports.stopBuzzing = function() {
  buzzer.stopSound();
  buzzer.stopSound(); // if called only once, buzzer doesn't completely stop
}

// Reset the alarm
exports.reset = function() {
  message("", 1);
  stopBuzzing();
}

exports.getTemperature = function() {
  return temp.value();
}
