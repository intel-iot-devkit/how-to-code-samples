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
var air = new (require("jsupm_gas").TP401)(0),
    speaker = new (require("jsupm_grovespeaker").GroveSpeaker)(5);

// The program is using the `superagent` module
// to make the remote calls to the data store
var request = require("superagent");

// Plays a chime sound using the Grove speaker
function chime() {
  speaker.playSound("a", true, "low");
  speaker.playSound("c", true, "low");
  speaker.playSound("g", true, "low");
}

// Store record in the remote datastore when the air quality
// level has exceeded the allowed threshold of safety
function sendAlert() {
  console.log("Air quality alert");

  if (!config.SERVER || !config.AUTH_TOKEN) {
    return;
  }

  function callback(err, res) {
    if (err) { return console.error("err:", err); }
    console.log("Datastore notified of air quality alert");
  }

  request
    .put(config.SERVER)
    .set("X-Auth-Token", config.AUTH_TOKEN)
    .send({ value: new Date().toISOString() })
    .end(callback);
}

// Alert user that the air quality level has exceeded
// the allowed threshold of safety
function alert() {
  sendAlert();
  chime();
}

// Main function checks the air quality every 1 second,
// then calls the `alert()` function if quality level has exceeded
// the allowed threshold of safety
function main() {
  var prev = 0,
      half = config.THRESHOLD / 2;

  setInterval(function() {
    var quality = air.getSample();
    if (prev <= config.THRESHOLD && quality > config.THRESHOLD && prev !== 0) { alert(); }
    prev = quality;
  }, 1000);
}

main();
