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

// The program is using the Node.js built-in `fs` module
// to load the config.json and html file used to configure the alarm time
var fs = require("fs");

// The program is using the Node.js built-in `path` module to find
// the file path to the html file used to configure the alarm time
var path = require("path");

// Load configuration data from `config.json` file. Edit this file
// to change to correct values for your configuration
var config = JSON.parse(
  fs.readFileSync(path.join(__dirname, "config.json"))
);

// The program is using the `superagent` module
// to make the remote calls to the Weather Underground API
var request = require("superagent");

// The program is using the `moment` module for easier time-based calculations,
// to determine when the alarm should be sounded.
var moment = require("moment");

// Initialize the hardware for whichever kit we are using
var board;
if (config.kit) {
  board = require("./" + config.kit + ".js");
} else {
  board = require('./grove.js');
}
board.init(config);

var datastore = require("./datastore");
const services = require("./services");

// State of the alarm
var current,
    alarm;

// Start the clock timer, then check every 50ms to see if is time to
// turn on the alarm
function startClock() {
  function after(a, b) { return a.isAfter(b, "second"); }
  function same(a, b) { return a.isSame(b, "second"); }

  setInterval(function() {
    var time = moment();

    // check if display needs to be updated
    if (after(time, current)) {
      board.message(time.format("h:mm:ss A"));
      if (same(current, alarm)) { startAlarm(); }
    }

    current = time;
  }, 50);
}

// Called to start the alarm when the time has come to get up
function startAlarm() {
  var tick = true;

  console.log("Alarm time");
  board.color("red");
  board.buzz();
  getWeather();

  var interval = setInterval(function() {
    board.color(tick ? "white" : "red");
    if (tick) { board.stopBuzzing(); } else { board.buzz(); }
    tick = !tick;
  }, 250);

  board.events.once("button-press", function() {
    clearInterval(interval);

    // notify how long alarm took to be silenced
    notify(moment().diff(alarm).toString());

    alarm = alarm.add(1, "day");

    board.color("white");
    board.stopBuzzing();
  });
}

// Display and then store record in the remote datastore and/or mqtt server
// of how long the alarm was ringing before it was turned off
function notify(duration) {
  console.log("Alarm duration (ms):" + duration);

  var payload = { value: duration };
  datastore.log(config, payload);
  services.log(config, payload);
}

// Call the remote Weather Underground API to check the weather conditions
// change the LOCATION variable to set the location for which you want.
function getWeather() {
  if (!config.WEATHER_API_KEY) { return; }

  var url = "http://api.wunderground.com/api/";

  url += config.WEATHER_API_KEY;
  url += "/conditions/q/CA/" + config.LOCATION + ".json";

  function display(err, res) {
    if (err) { return console.error("unable to get weather data", res.text); }
    var conditions = res.body.current_observation.weather;
    console.log("forecast:", conditions);
    board.message(conditions, 1);
  }

  request.get(url).end(display);
}

// Starts the built-in web server that serves up the web page
// used to set the alarm time
function server() {
  var app = require("express")();

  // Serve up the main web page used to configure the alarm time
  function index(res) {
    function serve(err, data) {
      if (err) { return console.error(err); }
      res.send(data);
    }
    fs.readFile(path.join(__dirname, "index.html"), {encoding: "utf-8"}, serve);
  }

  // styles for the web page
  function styles(req, res) {
    res.sendFile(path.join(__dirname, "styles.css"));
  }

  // Set new alarm time submitted by the web page using HTTP GET
  function get(req, res) {
    var params = req.query,
        time = moment();

    time.hour(+params.hour);
    time.minute(+params.minute);
    time.second(+params.second);

    if (time.isBefore(moment())) {
      time.add(1, "day");
    }

    alarm = time;

    index(res);
  }

  // Return the JSON data for the currently set alarm time
  function json(req, res) {
    if (!alarm) { return res.json({ hour: 0, minute: 0, second: 0 }); }

    res.json({
      hour: alarm.hour() || 0,
      minute: alarm.minute() || 0,
      second: alarm.second() || 0
    });
  }

  app.get("/", get);
  app.get("/styles.css", styles);
  app.get("/alarm.json", json);

  app.listen(3000);
}

// The main function makes sure the alarm buzzer is turned off,
// then starts listening for events coming from the connected hardware.
// the clock then starts ticking, and the built-in web server started up,
// so that the user can configure their desired wake up time.
// Lastly, the main function adjusts the brightness of the RGB LCD
// when the rotary knob is turned.
function main() {
  board.stopBuzzing();
  board.setupEvents();

  startClock();
  server();
}

main();
