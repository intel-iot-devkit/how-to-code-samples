/*
* Copyright (c) 2015 - 2016 Intel Corporation.
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

'use strict';

const mqtt = require('mqtt');
const fs = require("fs");

function publish(config, payload) {

    if (!config.services && !config.services.mqtt) {
        return;
    }

    let mqtt_hostname = config.services.mqtt.hostname;
    let mqtt_topic = config.services.mqtt.topic;

    let options = {
        clientId: config.services.mqtt.client_id,
        username: config.services.mqtt.username,
        password: config.services.mqtt.password
    };

    if (config.services.mqtt.cert && config.services.mqtt.key) {
        options.cert = fs.readFileSync(config.services.mqtt.cert);
        options.key = fs.readFileSync(config.services.mqtt.key);
    }

    const data = JSON.stringify({ d: payload });

    console.log('Publishing to MQTT service.');
    const client = mqtt.connect(mqtt_hostname, options);
    client.on('connect', function () {
        console.log('MQTT topic: {0}.', topic);
        client.publish(mqtt_topic, data);
        console.log('MQTT message published: {0}.', data);
        client.end();
    });

}

exports.module = {
    publish: publish
}