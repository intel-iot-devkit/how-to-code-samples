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
