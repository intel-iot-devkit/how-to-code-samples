/*
* Copyright (c) 2015-2016 Intel Corporation.
*
* Permission is hereby granted, free of charge, to any person ("User") obtaining
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

// The program is using the `mqtt` module
// to publish data to remote MQTT server
var mqtt = require('mqtt');

// The program is using the Node.js built-in `fs` module
// to load the SSL certificates, if needed
var fs = require("fs");

// Publish data to MQTT server
function publish(config, payload) {
  if (!config.MQTT_SERVER || !config.MQTT_CLIENTID) {
    return;
  }

  var data = {d: payload}
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

exports.log = publish;

exports.increment = function(config) {
  var payload = {counter: new Date().toISOString()};
  publish(config, payload);
};
