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

// Initialize the hardware devices
var mic = require("jsupm_mic");
var sound = new mic.Microphone(0),
    vibration = new (require("jsupm_ldt0028").LDT0028)(2),
    screen = new (require("jsupm_i2clcd").Jhd1313m1)(6, 0x3E, 0x62);

var datastore = require("./datastore");
var mqtt = require("./mqtt");

// Initialize the sound sensor
var ctx = new mic.thresholdContext();
ctx.averageReading = 0;
ctx.runningAverage = 0;
ctx.averagedOver = 2;

// Display and then store record in the remote datastore/mqtt server
// of how long the equipment being monitored was in use for
function notify(state) {
  console.log("Value: " + state + " " + new Date().toISOString());
  var payload = { value: state + " " + new Date().toISOString() };

  datastore.log(config, payload);
  mqtt.log(config, payload);
}

// Clears the I2C LCD display
function clear() {
  screen.setCursor(0, 0);
  screen.write("                ");
  screen.setColor(0, 0, 0);
}

// Main function, checks every 20ms to see if either vibrations or
// sounds are detected.
// If either are detected, and the program is not already in the
// process of notifying about the activity, it display a message
// and notifies the server.
function main() {
  var tripped = false;

  setInterval(function() {
    var movement = vibration.getSample() >= config.VIBRATION_THRESHOLD;

    var buffer = new mic.uint16Array(128),
        len = sound.getSampledWindow(2, 128, buffer);

    if (!len) { return; }

    var noise = sound.findThreshold(ctx, config.NOISE_THRESHOLD, buffer, len);

    if (movement && noise && !tripped) {
      notify("start");
      warn();
    }

    if (!(movement && noise) && tripped) {
      notify("stop");
      clear();
    }

    tripped = movement && noise;
  }, 20);
}

main();
