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
  var payload = {counter: 1};
  publish(config, payload);
};
