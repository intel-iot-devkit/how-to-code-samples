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
// to load the html file used to view the range finder status
var fs = require("fs");

// The program is using the Node.js built-in `path` module to find
// the file path to the html file used to view the range finder status
var path = require("path");

// Array to store range
var STATE = Array(360);

// Initialize the hardware devices
var ULN200XA = require("jsupm_uln200xa");
var dist = new (require("jsupm_rfr359f").RFR359F)(2),
    motor = new ULN200XA.ULN200XA(4096, 9, 10, 11, 12);

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

  app.get("/", index);
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
    console.log(degree, STATE[degree]);
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
