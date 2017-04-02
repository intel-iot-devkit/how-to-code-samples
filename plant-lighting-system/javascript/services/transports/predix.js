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

const fetch = require('node-fetch');
const uuid = require('uuid/v4');
const WebSocketClient = require('ws');
const querystring = require('querystring');

function authenticate(endpoint, zone_id, client_id, client_secret, username, password) {

    const token_endpoint = `${endpoint}/oauth/token`
    
    const data = querystring.stringify({
        grant_type: 'client_credentials',
        client_id: client_id,
        client_secret: client_secret,
        response_type: 'token'
    });
    
    return fetch(token_endpoint, {
        method: 'POST',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body: data
    });

}

function push(endpoint, zone_id, token, data) {
    
    const payload = JSON.stringify({
        messageId: uuid(),
        body: [
            {
                name: "sensor-data",
                datapoints: [
                    [
                        +new Date(),
                        JSON.stringify(data)
                    ]
                ]
            }
        ]
    });
    
    const client = new WebSocketClient(endpoint, {
        origin: 'https://intel-how-to-samples.example.org',
        headers: {
            'Predix-Zone-Id': zone_id,
            'Authorization': `Bearer ${token}`
        },
    });

    client.on('open', function() {
        console.log(`Predix Time Series payload: ${payload}`);
        client.send(payload, function(error) {
            if (error) {
                console.error(`Predix Time Series publish error: ${error}`);
            }
            else {
                console.log('Predix Time Series publish succeeded.')
            }
        });
    });

}

function publish(config, payload) {

    if (!config.services && !config.services.predix) {
        return;
    }

    const predix_uaa_client_id = config.services.predix.uaa_client_id;
    const predix_uaa_client_secret = config.services.predix.uaa_client_secret;
    const predix_uaa_url = config.services.predix.uaa_url;
    const predix_timeseries_zone_id = config.services.predix.timeseries_zone_id;
    const predix_timeseries_ingestion_url = config.services.predix.timeseries_ingest_url;

    if (!predix_uaa_client_id || !predix_uaa_client_secret || !predix_uaa_url || !predix_timeseries_zone_id || !predix_timeseries_ingestion_url) {
        console.error("Missing required Predix config values.");
        return;
    }

    console.log('Publishing to Predix Time Series service.');

    authenticate(predix_uaa_url, predix_timeseries_zone_id, predix_uaa_client_id, predix_uaa_client_secret)
        .then(response => response.json())
        .then(data => data.access_token)
        .then(token => push(predix_timeseries_ingestion_url, predix_timeseries_zone_id, token, payload));

}

module.exports = {
    publish: publish,
}