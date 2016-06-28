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

#ifndef MQTT_H_
#define MQTT_H_

#include <stdlib.h>
#include <iostream>
#include <sstream>

extern "C" {
  #include "MQTTClient.h"
  #include "MQTTClientPersistence.h"
}

#define QOS         1
#define TIMEOUT     10000L

/* Call mqtt server to log data
 *
 * Uses the following required ENV vars:
 *  MQTT_SERVER - protocol, host name, and port of the MQTT server
 *  MQTT_CLIENTID - MQTT client ID that will be used for publishing
 *  MQTT_TOPIC - topic that will be used for publishing MQTT messages
 *
 * Uses the following optional ENV vars:
 *  MQTT_USERNAME - username for authentication against the MQTT server
 *  MQTT_PASSWORD - password for authentication against the MQTT server
 *  MQTT_CERT - file name of the client certificate for authentication against the MQTT server
 *  MQTT_KEY - file name of the key file for the client certificate for authentication against the MQTT server
 *  MQTT_CA - file name of root Certificate Authority (CA) certificate for verifying the MQTT server
 */
void log_mqtt(std::string payload);

/* Call mqtt server to increment data
 *
 * Uses the following required ENV vars:
 *  MQTT_SERVER - protocol, host name, and port of the MQTT server
 *  MQTT_CLIENTID - MQTT client ID that will be used for publishing
 *  MQTT_TOPIC - topic that will be used for publishing MQTT messages
 *
 * Uses the following optional ENV vars:
 *  MQTT_USERNAME - username for authentication against the MQTT server
 *  MQTT_PASSWORD - password for authentication against the MQTT server
 *  MQTT_CERT - file name of the client certificate for authentication against the MQTT server
 *  MQTT_KEY - file name of the key file for the client certificate for authentication against the MQTT server
 *  MQTT_CA - file name of root Certificate Authority (CA) certificate for verifying the MQTT server
 */
void increment_mqtt();

#endif /* MQTT_H_ */
