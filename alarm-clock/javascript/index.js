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

function main() {

  if (config.kit == "dfrobot"){
    var dfrobot = require('./index-dfrobot.js');
    dfrobot.stopBuzzing();
    dfrobot.setupEvents();
    dfrobot.startClock();
    dfrobot.server();
  } else {
    var seeed = require('./index-seeedstudio.js');
    seeed.stopBuzzing();
    seeed.setupEvents();
    seeed.startClock();
    seeed.server();
    seeed.adjBrightness();
  }
  
}

main();
