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
var touch = new (require("jsupm_ttp223").TTP223)(4),
    buzzer = new (require("jsupm_buzzer").Buzzer)(5),
    screen = new (require("jsupm_i2clcd").Jhd1313m1)(6, 0x3E, 0x62);

// The program is using the `superagent` module
// to make the remote calls to the data store
var request = require("superagent");

// Colors used for the RGB LED
var colors = { green: [0, 255, 0], white: [255, 255, 255] };

// Writes a message to the I2C LCD display
function message(string, color) {
  // pad string to avoid display issues
  while (string.length < 16) { string += " "; }

  screen.setCursor(0, 0);
  screen.write(string);
  screen.setColor.apply(screen, color || colors.white);
}

// Reset the state of the doorbot
function reset() {
  message("doorbot ready");
  buzzer.setVolume(0.5);

  // we call #stopSound twice because otherwise buzzer may not actually stop
  buzzer.stopSound();
  buzzer.stopSound();
}

// Increment the datastore server's count of doorbell rings
function increment() {
  console.log("doorbell ring");

  if (!config.SERVER || !config.AUTH_TOKEN) {
    return;
  }

  function callback(err, res) {
    if (err) { return console.error("err:", err); }
    console.log("total # of doorbell rings:", res.body.value);
  }

  request
    .get(config.SERVER)
    .set("X-Auth-Token", config.AUTH_TOKEN)
    .end(callback);
}

// Logs a doorbell ring, prints a message to the display,
// and rings the buzzer
function dingdong() {
  increment();
  message("ding dong!", colors.green);
  buzzer.playSound(2600, 0);
}

// Main function, resets the doorbot, and then checks every
// 50ms to see if anyone has rung the bell
function main() {
  reset();

  var prev = false;

  setInterval(function() {
    var current = touch.isPressed();
    if (current && !prev) { dingdong(); }
    if (!current && prev) { reset(); }
    prev = current;
  }, 50);
}

main();
