/*
* Copyright (c) 2015-2016 Intel Corporation.
*
* Permission is hereby granted, free of charge, to any person (“User”) obtaining
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

var datastore = require("./datastore");
var mqtt = require("./mqtt");

// Initialize the hardware devices
var screen = new (require("jsupm_i2clcd").SSD1308)(0, 0x3C),
    accel = new (require("jsupm_adxl345").Adxl345)(0);

// The program is using the `twilio` module
// to make the remote calls to Twilio service
// to send SMS alerts
var twilio;
if (config.TWILIO_ACCT_SID && config.TWILIO_AUTH_TOKEN) {
  twilio = require("twilio")(config.TWILIO_ACCT_SID,
                             config.TWILIO_AUTH_TOKEN);
}


// Reads the latest values from the accelerometer
function get() {
  accel.update();
  var force = accel.getAcceleration();
  function item(n) { return force.getitem(n); }
  var val = { x: item(0), y: item(1), z: item(2) };
  return val;
}

// Checks to see if it looks like the wearer has fallen down
function detectFall(prev, current) {
  if (current === undefined) { return false; }
  if (prev.x - current.x >= 1) { return true; }
  if (prev.y - current.y >= 1) { return true; }
  if (prev.z - current.z >= 1) { return true; }
  return false;
}

// Displays a message on the OLED display
function message(msg) {
  screen.clear();
  screen.home();
  screen.write(msg);
}

// Sends an alert using SMS if it looks like a fall
// has occurred
function alertSMS() {
  if (!config.TWILIO_ACCT_SID || !config.TWILIO_AUTH_TOKEN) {
    return;
  }
  var opts = { to: config.NUMBER_TO_SEND_TO,
               from: config.TWILIO_OUTGOING_NUMBER,
               body: "fall detected" };

  twilio.sendMessage(opts, function(err, response) {
    if (err) { return console.error("err:", err); }
    console.log("SMS sent", response);
  });
}

// Sends SMS and logs record in the remote datastore/mqtt server
// that a fall may have occurred
function alert() {
  console.log("Fall Detected!");
  message("Fall Detected!");

  alertSMS();

  var payload = { value: new Date().toISOString() };
  datastore.log(config, payload);
  mqtt.log(config, payload);
}

// The main function starts reading the accelerometer
// every 250 ms to check to see if it appears that the wearer
// may have fallen down, and if so, it alerts that help
// is needed
function main() {
  var prev = { x: 0, y: 0, z: 0 };

  setInterval(function() {
    var current = get();
    if (detectFall(prev, current)) { alert(); }
    prev = current;
  }, 250);
}

main();
