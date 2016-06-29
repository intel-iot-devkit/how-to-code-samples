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

#include "mqtt.h"

void log_mqtt(std::string payload) {
  if (!getenv("MQTT_SERVER")) {
    return;
  }

  MQTTClient client;
  MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
  MQTTClient_message pubmsg = MQTTClient_message_initializer;
  MQTTClient_deliveryToken token;
  int rc;

  MQTTClient_create(&client, getenv("MQTT_SERVER"), getenv("MQTT_CLIENTID"),
     MQTTCLIENT_PERSISTENCE_NONE, NULL);
  conn_opts.keepAliveInterval = 20;
  conn_opts.cleansession = 1;
  conn_opts.username = getenv("MQTT_USERNAME");
  conn_opts.password = getenv("MQTT_PASSWORD");

  MQTTClient_SSLOptions sslOptions = MQTTClient_SSLOptions_initializer;
  if (getenv("MQTT_CERT") && getenv("MQTT_KEY") && getenv("MQTT_CA")) {
    sslOptions.keyStore = getenv("MQTT_CERT");
    sslOptions.privateKey = getenv("MQTT_KEY");
    sslOptions.trustStore = getenv("MQTT_CA");
  } else {
    sslOptions.enableServerCertAuth = false;
  };
  conn_opts.ssl = &sslOptions;

  if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
  {
    std::cout << "Failed to connect to MQTT server, return code:" << rc << std::endl;
    return;
  }

  std::stringstream json;
  json << "{\"d\": " << payload << " }";
  std::string jsonStr = json.str();

  pubmsg.payload = &jsonStr;
  pubmsg.payloadlen = json.str().length();
  pubmsg.qos = QOS;
  pubmsg.retained = 0;
  MQTTClient_publishMessage(client, getenv("MQTT_TOPIC"), &pubmsg, &token);
  std::cout << "Publishing to MQTT server on" << getenv("MQTT_TOPIC") << std::endl;
  std::cout << json.str() << std::endl;
  rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
  std::cout << "MQTT message delivered" << std::endl;
  MQTTClient_disconnect(client, 10000);
  MQTTClient_destroy(&client);
}

void increment_mqtt() {
  std::string payload = "{\"counter\": \"1\" }";
  log_mqtt(payload);
}
