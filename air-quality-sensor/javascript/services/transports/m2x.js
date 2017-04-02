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
const uuid = require("uuid/v4");

const m2x_mqtt_endpoint = 'mqtts://api-m2x.att.com';

function publish(config, payload) {
    
    if (!config.services && !config.services.m2x) {
        return;
    }

    const m2x_api_key = config.services.m2x.api_key;
    const m2x_device_id = config.services.m2x.device_id;
    const m2x_stream_id = config.services.m2x.stream_id;

    if (!m2x_api_key || !m2x_device_id || !m2x_stream_id) {
        console.error("Missing required M2X config values.");
        return;
    }

    const data = JSON.stringify({
        id: uuid(),
        method: "POST",
        resource: `/v2/devices/${m2x_device_id}/streams/${m2x_stream_id}/values`,
        body: {
            values: [{
                timestamp: new Date().toISOString(),
                value: JSON.stringify(payload)
            }]
        }
    });

    const m2x_topic = `m2x/${m2x_api_key}/requests`;

    const options = {
        username: m2x_api_key,
    }

    console.log("Publishing to M2X MQTT service.");
    const client = mqtt.connect(m2x_mqtt_endpoint, options);
    client.on('connect', function () {
        console.log(`M2X MQTT topic: ${m2x_topic}`);
        client.publish(m2x_topic, data);
        console.log(`M2X MQTT message payload: ${data}`);
        client.end();
    });
}

module.exports = {
    publish: publish,
}