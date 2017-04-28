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

let services = null;

function servicesInit(config) {

    let transports = []
    
    if (config.services) {
        
        if (config.services.m2x) {
            const m2x = require('./transports/m2x');
            transports.push(m2x);
        }

        if (config.services.predix) {
            const predix = require('./transports/predix');
            transports.push(predix);
        }

        if (config.services.sap) {
            const sap = require('./transports/sap');
            transports.push(sap);
        }

        if (config.services.mqtt) {
            const mqtt = require('./transports/mqtt');
            transports.push(mqtt);
        }

    }

    return transports;

}

function publish(config, payload) {

    if (!services) {
        services = servicesInit(config);
    }
    
    services.forEach((service, index, obj) => service.publish(config, payload));

}

function increment(config) {
    const payload = { counter: new Date().toISOString() };
    publish(config, payload);
}

module.exports = {
    log: publish,
    increment: increment
};