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

var datastore = require("./datastore");
var mqtt = require("./mqtt");

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

// Increment the datastore/mqtt server count of doorbell rings
function increment() {
  console.log("doorbell ring");

  datastore.increment(config);
  mqtt.increment(config);
}

// Publish data to MQTT server
function incrementMQTT() {
  if (!config.MQTT_SERVER || !config.MQTT_CLIENTID) {
    return;
  }

  var data = {d: {count: 1}};
  var options = {
    clientId: config.MQTT_CLIENTID,
    username: config.MQTT_USERNAME,
    password: config.MQTT_PASSWORD
  };

  if (config.MQTT_CERT && config.MQTT_KEY) {
    options.cert = fs.readFileSync(config.MQTT_CERT);
    options.key = fs.readFileSync(config.MQTT_KEY);
  }

  console.log("Connecting to MQTT server...");
  var client  = mqtt.connect(config.MQTT_SERVER, options);
  client.on('connect', function () {
    client.publish(config.MQTT_TOPIC, JSON.stringify(data));
    console.log("MQTT message published:", data);
    client.end();
  });
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
