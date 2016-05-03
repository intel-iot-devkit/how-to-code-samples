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

// Set the initial default target temperature
var TARGET_TEMP = config.TARGET_TEMP;

// Initialize the hardware devices
var temp = new (require("jsupm_otp538u").OTP538U)(0, 1),
    flame = new (require("jsupm_yg1006").YG1006)(4),
    speaker = new (require("jsupm_grovespeaker").GroveSpeaker)(5);

var datastore = require("./datastore");
var mqtt = require("./mqtt");

// Plays an audible alarm when the temperature has exceeded
// the target temperature
function tempAlarm() {
  speaker.playSound("a", true, "low");
  speaker.playSound("c", true, "low");
  speaker.playSound("d", true, "low");
  speaker.playSound("b", false, "low");
}

// Plays an audible alarm when the flame sensor indicates
// a possible fire
function fireAlarm() {
  var i = 0;
  while (i < 10) {
    speaker.playSound("a", true, "high");
    speaker.playSound("c", true, "high");
    speaker.playSound("g", true, "med");
    i++;
  }
}

// Log record in the remote datastore of the current temperature
// every 1 minute
function log() {
  function notify() {
    console.log(temp.objectTemperature());
    var payload = { value: temp.objectTemperature() };

    datastore.log(config, payload);
    mqtt.log(config, payload);
  }

  notify();

  // log once a minute
  setInterval(notify, 60 * 1000);
}

// Monitors for both exceeding the target temperature, or a
// fire condition
function monitor() {
  var prev = { temp: 0, fire: false },
      logging = false;

  setInterval(function() {
    var temperature = temp.objectTemperature(),
        fire = flame.flameDetected();

    if (!logging && temperature >= TARGET_TEMP) {
      logging = true;
      log();
    }

    if (prev.temp <= TARGET_TEMP && temperature > TARGET_TEMP) { tempAlarm(); }
    if (!prev.fire && fire) { fireAlarm(); }

    prev.temp = temperature;
    prev.fire = fire;
  }, 1000);
}

// Starts the built-in web server that serves up the web page
// used to set the desired target temperature
function server() {
  var app = require("express")();

  // Set target temperature if passed in via params
  app.use(function(req, res, next) {
    if (req.query.temp) { TARGET_TEMP = +req.query.temp; }
    next();
  });

  // Serve up the main web page used to configure the temperature
  function index(req, res) {
    function serve(err, data) {
      if (err) { return console.error(err); }
      res.send(data);
    }

    fs.readFile(path.join(__dirname, "index.html"), {encoding: "utf-8"}, serve);
  }

  // Return the JSON data for the currently set target temperature
  function json(req, res) {
    res.json({ temp: TARGET_TEMP });
  }

  app.get("/", index);
  app.get("/temp.json", json);

  app.listen(process.env.PORT || 3000);
}

// The main function starts monitoring the connected hardware to
// check if the temperature exceeds the desired target, or there
// is a fire detected.
// It then starts the built-in web server, so the user can configure
// their desired target temperature.
function main() {
  monitor();
  server();
}

main();
