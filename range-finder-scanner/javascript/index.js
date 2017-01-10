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
// to load the html file used to view the range finder status
var fs = require("fs");

// The program is using the Node.js built-in `path` module to find
// the file path to the html file used to view the range finder status
var path = require("path");

// Load configuration data from `config.json` file. Edit this file
// to change to correct values for your configuration
var config = JSON.parse(
  fs.readFileSync(path.join(__dirname, "config.json"))
);

// Initialize the hardware devices
var mraa = require("mraa");
// pins
var rangerPin = 2,
    stepper1input1 = 9, stepper1input2 = 10,
    stepper1input3 = 11, stepper1input4 = 12;

if (config.platform == "firmata") {
  // open connection to firmata
  mraa.addSubplatform(mraa.GENERIC_FIRMATA, "/dev/ttyACM0");

  rangerPin += 512;
  stepper1input1 += 512;
  stepper1input2 += 512;
  stepper1input3 += 512;
  stepper1input4 += 512;
}

// Array to store range
var STATE = Array(360);

// Initialize the hardware devices
var ULN200XA = require("jsupm_uln200xa");
var dist = new (require("jsupm_rfr359f").RFR359F)(rangerPin),
    motor = new ULN200XA.ULN200XA(4096, stepper1input1, stepper1input2, stepper1input3, stepper1input4);

// Starts the built-in web server for the web page
// used to view the ranger finder status
function server() {
  var app = require("express")();

  // Serve up the main web page used to view range finder status
  function index(req, res) {
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

  app.get("/", index);
  app.get("/styles.css", styles);
  app.get("/data.json", function(req, res) { res.json(STATE); });

  app.listen(process.env.PORT || 3000);
}

// Moves the stepper motor for a predetermined distance
function move() {
  motor.setSpeed(5);
  motor.setDirection(0);
  motor.stepperSteps(11);
}

// Sweeps the stepper motor, to be able to get the range sensor
// reading for all 360 degress around the robot
function sweep() {
  var degree = 0;

  function work() {
    STATE[degree] = dist.objectDetected();
    console.log(degree, "degrees", STATE[degree]);
    degree = degree === 359 ? 0 : degree + 1;
    move();
    setTimeout(work, 10);
  }

  work();
}

// The main function calls `server()` to start up
// the built-in web server used to view the range sensor data.
// It also calls the `sweep()` function which sweeps the stepper
// motor in a circle and repeatedly reads the range sensor.
function main() {
  server();
  sweep();
}

main();
