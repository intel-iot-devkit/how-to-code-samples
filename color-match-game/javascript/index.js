/*
* Copyright (c) 2015 - 2017 Intel Corporation.
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

// Initialize the hardware for whichever kit we are using
var board;
if (config.kit) {
  board = require("./" + config.kit + ".js");
} else {
  board = require('./grove.js');
}
board.init(config);

// colors
var COLORS = ["red", "blue", "green", "yellow"]

// State of the game
var gameLevel=0,
    gameMove=0,
    colorSequence=[];

function startGame() {
  console.log("Starting game...");

  gameLevel = 0;
  gameMove = 0;
  colorSequence = [getRandomInt(0, 3)];

  logLevel();
  playSequence();
}

function nextLevel() {
  console.log("Next level...");

  gameLevel += 1;
  gameMove = 0;
  colorSequence.push(getRandomInt(0, 3));

  logLevel();
  playSequence();
}

function matchColor(guess) {
  var expectedColor = colorSequence[gameMove];
  var result = (guess == expectedColor);
  console.log("Color match guess:", guess, ", expected:", expectedColor, ", result:", result)
  return result
}

function endLevel() {
  gameMove += 1;
  if (gameMove == colorSequence.length) {
    console.log("Reached game level end.");
    return true;
  }
  console.log("Continue game level.");
  return false;
}

function logLevel() {
  console.log("Game level:", gameLevel);
  console.log("Game move:", gameMove);
  console.log("Color sequence:", colorSequence);
}

function playSequence() {
  var colorLength = colorSequence.length;
  var time = 1;
  for (var i = 0; i < colorLength; i++) {
    play(i, time);
    time += 2;
  }
}

function play(count, offset) {
  setTimeout(function() {
    board.color(COLORS[colorSequence[count]]);
  }, 1000 * offset);
  setTimeout(function() {
    board.color("white");
  }, 1000 * (offset + 1));
}

function getRandomInt(min, max) {
  min = Math.ceil(min);
  max = Math.floor(max);
  return Math.floor(Math.random() * (max - min + 1)) + min;
}

// Starts the built-in web server that serves up the web page
// used to play the game
function server() {
  var express = require("express")
  var app = express();

  app.use(express.static(__dirname + "/public"))

  function color(req, res) {
    var idx = COLORS.indexOf(req.params.color);
    if (idx == -1) {
      res.status(404).send("Color Not Found");
      return;
    }

    if (matchColor(idx)) {
      if (endLevel()) {
        res.status(204).send("Color Match - End Level");
        return;
      }
      res.status(200).send("Color Match");
      return;
    } else {
      res.status(406).send("Color Mismatch");
      return;
    }
  }

  function action(req, res) {
    if (req.params.action == "startGame") {
      startGame();
      res.status(200).send("Action Executed");
    } else if (req.params.action == "nextLevel") {
      nextLevel();
      res.status(200).send("Action Executed");
    } else {
      res.status(404).send("Unknown Action");
    }
  }

  app.get("/api/colors/:color", function(req, res) {
    color(req, res);
  });

  app.get("/api/actions/:action", function(req, res) {
    action(req, res);
  });

  app.listen(3000);
}

// The main function makes sure that the game server is started
function main() {
  server();
}

main();
