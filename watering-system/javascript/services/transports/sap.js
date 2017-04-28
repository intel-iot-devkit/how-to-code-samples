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

function push(endpoint, device_id, message_type_id, token, data) {

    const data_endpoint = `${endpoint}/${device_id}`

    console.log(`SAP MMS data endpoint: ${data_endpoint}`);
    
    const mms_headers = {
        'Authorization': `Bearer ${token}`,
        'Content-Type': 'application/json;charset=utf-8'
    };

    const mms_body = {
        'mode': 'sync',
        'messageType': `${message_type_id}`,
        'messages': [{
            'sensor': 'IntelHowToSample',
            'value': JSON.stringify(data),
            'timestamp': +Math.floor(new Date()/1000)
        }]
    };

    console.log(`SAP MMS payload: ${JSON.stringify(mms_body)}`);

    return fetch(data_endpoint, {
        method: 'POST',
        headers: mms_headers,
        body: JSON.stringify(mms_body)
    });

}

function publish(config, payload) {

    if (!config.services && !config.services.sap) {
        return;
    }

    const sap_mms_endpoint = config.services.sap.mms_endpoint;
    const sap_device_id = config.services.sap.device_id;
    const sap_message_type_id = config.services.sap.message_type_id;
    const sap_oauth_token = config.services.sap.oauth_token;

    if (!sap_mms_endpoint || !sap_device_id || !sap_message_type_id || !sap_oauth_token) {
        console.error('Missing required SAP config values.');
        return;
    }

    console.log('Publishing to SAP MMS service.');

    push(sap_mms_endpoint, sap_device_id, sap_message_type_id, sap_oauth_token, payload)
        .then(
            response => {
                
                if (response.ok) {
                    console.log("SAP MMS publish succeeded.")
                }
                else {
                    response.text().then(data => {
                        console.log(`SAP MMS response: ${response.status} - ${response.statusText} | ${data}`);
                    });
                }
            },
            error => console.log(`SAP MMS publish error: ${error}`)
        );

}

module.exports = {
    publish: publish,
}