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

var DATA = [];

// Initialize the hardware devices
var ts = require("jsupm_at42qt1070");
var I2C_BUS = ts.AT42QT1070_I2C_BUS;
var DEFAULT_I2C_ADDR = ts.AT42QT1070_DEFAULT_I2C_ADDR;
var touch = new ts.AT42QT1070(I2C_BUS, DEFAULT_I2C_ADDR),
    bmp = new (require("jsupm_bmpx8x").BMPX8X)(0, 0x77),
    screen = new (require("jsupm_i2clcd").SSD1308)(0, 0x3C);

// Checks the readings from the digital barometer
// every 1 second, and stores to the DATA array
function monitor() {
  setInterval(function() {
    var data = {
      pressure: bmp.getPressure(),
      temp: bmp.getTemperature(),
      altitude: bmp.getAltitude(),
      sealevel: bmp.getSealevelPressure()
    };

    console.log(data);
    DATA.push(data);
  }, 1000);
}

// Starts the built-in web server for the JSON data
// from the stored barometer readings
function server() {
  var app = require("express")();
  app.get("/", function(req, res) { res.json(DATA); });
  app.listen(3000);
}

// Display the most recent barometer reading on
// the OLED display
function showLatest() {
  var data = DATA[DATA.length - 1];

  screen.clear();

  Object.keys(data).forEach(function(name, idx) {
    screen.setCursor(idx, 0);
    screen.write(name + ": " + data[name]);
  });

  setTimeout(screen.clear.bind(screen), 10 * 1000);
}

// Check every 100 ms to see if the touch button is being
// pushed
function button() {
  var prev = false;

  setInterval(function() {
    touch.updateState();

    var pressed = false,
        buttons = touch.getButtons();

    for (var i = 0; i < 7; i++) {
      if (buttons & (1 << i)) { pressed = true; }
    }

    if (!prev && pressed) { showLatest(); }

    prev = pressed;
  }, 100);
}

// The main function calls the `monitor()` function
// to begin reading the digital barometer every second
// and storing that data to a local array.
// Then it calls `server()` to start up
// the built-in web server used to retrieve the data.
// Lastly, it calls `button()` to start reading the touch
// sensor every 50 ms, to display the latest barometer
// reading on the OLED display when pressed.
function main() {
  monitor();
  server();
  button();
}

main();
