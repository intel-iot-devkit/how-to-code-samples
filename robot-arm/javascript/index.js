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

var CLOCKWISE = 1,
    COUNTERCLOCKWISE = 2;

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
var joyPinX = 2, joyPinY = 3,
    stepper1input1 = 9, stepper1input2 = 10,
    stepper1input3 = 11, stepper1input4 = 12,
    stepper2input1 = 4, stepper2input2 = 5,
    stepper2input3 = 6, stepper2input4 = 7,
    voltageAdjust = 1.0;

if (config.platform == "firmata") {
  // open connection to firmata
  mraa.addSubplatform(mraa.GENERIC_FIRMATA, "/dev/ttyACM0");

  joyPinX += 512;
  joyPinY += 512;
  stepper1input1 += 512;
  stepper1input2 += 512;
  stepper1input3 += 512;
  stepper1input4 += 512;
  stepper2input1 += 512;
  stepper2input2 += 512;
  stepper2input3 += 512;
  stepper2input4 += 512;
  voltageAdjust = 1.33;
}

var ULN200XA = require("jsupm_uln200xa");
var thumb = new (require("jsupm_joystick12").Joystick12)(joyPinX, joyPinY),
    step1 = new ULN200XA.ULN200XA(4096, stepper1input1, stepper1input2, stepper1input3, stepper1input4),
    step2 = new ULN200XA.ULN200XA(4096, stepper2input1, stepper2input2, stepper2input3, stepper2input4);

// Moves the stepper motor
function move(stepper, dir, speed, steps) {
  console.log("move", dir || CLOCKWISE, steps || 1020);
  stepper.setSpeed(5);
  stepper.setDirection(dir || CLOCKWISE);
  stepper.stepperSteps(steps || 1020);
}

// Helper function to scale to needed -1..1 range
function scale(n) {
  // convert down to 0..1
  var val = (n - -0.5) / -0.4;
  if (val > 1) { val = 1; }
  if (val < 0) { val = 0; }

  // and then to -1..1 and round
  return Math.round(val * 2 - 1);
}

// Starts the built-in web server for the web page
// used to view or control the arm
function server() {
  var app = require("express")();

  // Serve up the main web page used for the robot arm
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

  // Handler for each of the RESTful endpoints to control the arm
  function handle(stepper, dir) {
    return function(req, res) {
      res.send("done");
      move(stepper, dir);
    };
  }

  // motor 1
  app.post("/one-cw", handle(step1, CLOCKWISE));
  app.post("/one-ccw", handle(step1, COUNTERCLOCKWISE));

  // motor 2
  app.post("/two-cw", handle(step2, CLOCKWISE));
  app.post("/two-ccw", handle(step2, COUNTERCLOCKWISE));

  app.listen(process.env.PORT || 3000);
}

// The main function calls `server()` to start up
// the built-in web server used to control the arm.
// It then starts reading the joystick every 50 ms, and using
// that data to control the stepper motors for the arm.
function main() {
  var x, y;

  server();

  setInterval(function() {
    x = scale(thumb.getXInput() * voltageAdjust);
    y = scale(thumb.getYInput() * voltageAdjust);

    if (x === 1) { move(step1, CLOCKWISE); }
    if (x === -1) { move(step1, COUNTERCLOCKWISE); }

    if (y === 1) { move(step2, CLOCKWISE); }
    if (y === -1) { move(step2, COUNTERCLOCKWISE); }
  }, 100);
}

main();
