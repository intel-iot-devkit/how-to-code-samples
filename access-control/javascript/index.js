/*
* Copyright (c) 2015-2016 Intel Corporation.
*
* Permission is hereby granted, free of charge, to any person ("User") obtaining
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

// Colors used for the RGB LCD display
var COLORS = {
  blue: [0, 0, 255],
  red: [255, 0, 0],
  white: [255, 255, 255]
};

var TIMEOUT = 30 * 1000;

var CODE = config.CODE || "1234";

var VALIDATED = false,
    EXPECTING_CODE = false,
    ALARM_IN_PROGRESS = false;

// Initialize the hardware devices
var screen = new (require("jsupm_i2clcd").Jhd1313m1)(6, 0x3E, 0x62),
    motion = new (require("jsupm_biss0001").BISS0001)(4);

// Store record in the remote datastore and/or mqtt server
// when access control event has occurred
function log(event) {
  var msg = new Date().toISOString() + " " + event;
  console.log(msg);

  var payload = { value: msg };
  datastore.log(config, payload);
  mqtt.log(config, payload);
}

// Displays a message on the RGB LED
function message(string) {
  // pad string to avoid display issues
  while (string.length < 16) { string += " "; }

  screen.setCursor(0, 0);
  screen.write(string);
}

// Sets the background color on the RGB LED
function color(str) {
  screen.setColor.apply(screen, COLORS[str] || COLORS.white);
}

// Turns on the alarm
function startAlarm() {
  log("alarm-starting");
  ALARM_IN_PROGRESS = true;

  color("red");
  message("ALARM");
}

// Tiurns off the alarm
function stopAlarm() {
  log("alarm-stopping");
  ALARM_IN_PROGRESS = false;
}

// Pauses looking for motion for 30 seconds
function wait() {
  log("waiting");
  color("white");
  message("Waiting 30s");
  setTimeout(lookForMotion, TIMEOUT);
}

// Motion detected, listen for code and set off alarm
// if we don't get it in time
function alert() {
  log("motion-detected");

  color("blue");
  message("ALERT");

  EXPECTING_CODE = true;

  var timeout = setTimeout(startAlarm, TIMEOUT);

  // check if code was entered - if so, we'll transition back to scanning
  var interval = setInterval(function() {
    if (EXPECTING_CODE && VALIDATED) {
      log("validated-entry");
      VALIDATED = false;
      EXPECTING_CODE = false;

      if (ALARM_IN_PROGRESS) { stopAlarm(); }

      clearTimeout(timeout);
      clearInterval(interval);
      wait();
    }
  }, 100);
}

// Check the motion sensor every 1 second
function lookForMotion() {
  var prev = false,
      interval;

  log("looking-for-motion");

  color("white");
  message("READY");

  interval = setInterval(function() {
    var movement = motion.value();

    if (!prev && movement && !ALARM_IN_PROGRESS) {
      clearInterval(interval);
      alert();
    }

    prev = movement;
  }, 1000);
}

// Starts the built-in web server for the web page
// used to enter code after triggering the motion sensor
function server() {
  var app = require("express")();

  // Serve up the main web page used to enter code after triggering
  // motion sensor
  function index(req, res) {
    function serve(err, data) {
      if (err) { return console.error(err); }
      res.send(data);
    }

    fs.readFile(path.join(__dirname, "index.html"), {encoding: "utf-8"}, serve);
  }

  // Called by the web page to submit the access code and "defuse"
  // the alarm system
  function defuse(req, res) {
    if (req.query.code === CODE && EXPECTING_CODE) { VALIDATED = true; }
    if (req.query.code !== CODE) { log("invalid-code " + req.query.code); }
    res.send("");
  }

  app.get("/", index);
  app.get("/alarm", defuse);

  app.listen(3000);
}

// The main function calls `server()` to start up
// the built-in web server used to enter the access code
// after triggering the alarm.
// It also calls the `lookForMotion()` function which monitors
// the motion sensor.
function main() {
  server();
  lookForMotion();
}

main();
