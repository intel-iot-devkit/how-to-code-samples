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
var ULN200XA = require("jsupm_uln200xa");
var lineFinder = new (require("jsupm_grovelinefinder").GroveLineFinder)(2),
    right = new ULN200XA.ULN200XA(4096, 9, 10, 11, 12),
    left = new ULN200XA.ULN200XA(4096, 4, 5, 6, 7);

// Helper function indicates we found a line
function line() {
  return lineFinder.blackDetected();
}

// Moves any of the stepper motors
function move(stepper) {
  return function(steps, dir, speed) {
    stepper.setSpeed(speed || 5);
    stepper.setDirection(dir || config.CLOCKWISE);
    stepper.stepperSteps(steps || 1024);
  };
}

// Defines helper functions to move specific stepper motor
right.move = move(right);
left.move = move(left);

// Find a line
function findLine() {
  var steps = 512;
  console.log("finding line");

  while (!line()) {
    left.move(steps);
    steps += 512;

    right.move(steps);
    steps += 512;
  }
}

// Move the robot forward
function moveForward() {
  console.log("moving forward");
  left.move(1024);
  right.move(1024);
}

// Store record in the remote datastore when robot is active
function log(callback) {
  console.log("active");

  var payload = { value: new Date().toISOString() };
  datastore.log(config, payload);
  mqtt.log(config, payload);
}

// The main function repeatedly calls `findLine()`
// to try to locate a black line on the ground using the sensor.
// It then logs that the robot is active to the remote datastore,
// and tries to move forward.
function main() {
  for (;;) {
    findLine();
    log(moveForward);
  }
}

main();
