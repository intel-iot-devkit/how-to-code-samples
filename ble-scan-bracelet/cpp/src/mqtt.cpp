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

bool mqtt_initialized = false;
MQTTClient client;
MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
MQTTClient_message pubmsg = MQTTClient_message_initializer;
MQTTClient_deliveryToken token;
MQTTClient_SSLOptions sslOptions = MQTTClient_SSLOptions_initializer;

void init_mqtt() {
  if (mqtt_initialized) {
    return;
  }

  MQTTClient_create(&client,
	getenv("MQTT_SERVER"),
	getenv("MQTT_CLIENTID"),
    MQTTCLIENT_PERSISTENCE_NONE,
    NULL);

  // connection options
  conn_opts.keepAliveInterval = 20;
  conn_opts.cleansession = 1;

  if (getenv("MQTT_USERNAME")) {
	   conn_opts.username = getenv("MQTT_USERNAME");
  }

  if (getenv("MQTT_PASSWORD")) {
	   conn_opts.password = getenv("MQTT_PASSWORD");
  }

  //ssl options
  if (getenv("MQTT_CERT") && getenv("MQTT_KEY") && getenv("MQTT_CA")) {
    sslOptions.keyStore = getenv("MQTT_CERT");
    sslOptions.privateKey = getenv("MQTT_KEY");
    sslOptions.trustStore = getenv("MQTT_CA");
  } else {
    sslOptions.enableServerCertAuth = false;
  };
  conn_opts.ssl = &sslOptions;

  mqtt_initialized = true;
};

void close_mqtt() {
  if (mqtt_initialized) {
    std::cout << "Closing MQTT..." << std::endl;
    MQTTClient_destroy(&client);
  }
};

void log_mqtt(std::string payload) {
  if (!getenv("MQTT_SERVER")) {
    return;
  }

  init_mqtt();

  auto mqtt_topic = std_getenv("MQTT_TOPIC");

  int rc;

  if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
  {
    std::cout << "Failed to connect to MQTT server, return code:" << rc << std::endl;
    return;
  }

  Json::Value root;
  
  auto mqtt_service_name = std_getenv("MQTT_SERVICE_NAME");
  if (!mqtt_service_name.empty())
  {
    auto str_service_name(mqtt_service_name);
    MqttServiceName service_name = map_mqtt_service_name(str_service_name);
    switch (service_name)
    {
      case MqttServiceName::M2X:
      {
        auto m2x_api_key = std_getenv("MQTT_M2X_API_KEY");
        auto m2x_device_id = std_getenv("MQTT_M2X_DEVICE_ID");
        auto m2x_stream_id = std_getenv("MQTT_M2X_STREAM_ID");

        if (m2x_api_key.empty() || m2x_device_id.empty() || m2x_stream_id.empty())
        {
          std::cout << "Missing MQTT M2X config values." << std::endl;
          return;
        }
        
        std::stringstream m2x_topic;
        m2x_topic << "m2x/" << m2x_api_key << "/requests";
        mqtt_topic = m2x_topic.str();
        
        // m2x data payload
        root["id"] = random_string();
        root["method"] = "POST";

        std::stringstream m2x_resource;
        m2x_resource << "/v2/devices/" << m2x_device_id << "/streams/" << m2x_stream_id << "/values";
        root["resource"] = m2x_resource.str();

        Json::Value body_values(Json::arrayValue);
        Json::Value body_value;
        time_t now;
        time(&now);
        char time_buf[sizeof "2017-02-12T07:07:07Z"];
        strftime(time_buf, sizeof time_buf, "%FT%TZ", gmtime(&now));
        body_value["timestamp"] = time_buf;
        body_value["value"] = payload;
        body_values.append(body_value);
        root["body"]["values"] = body_values;

        break;
      }
      
      case MqttServiceName::None:
      default:
        std::cout << "Failed to parse MQTT service config." << std::endl;
        return;
    }
  }
  else
  {
    root["d"] = payload;
  }

  Json::FastWriter writer;
  auto serialized_payload = writer.write(root);

  std::vector<char> writable_serialized_payload(serialized_payload.begin(), serialized_payload.end());

  pubmsg.payload = &writable_serialized_payload[0];
  pubmsg.payloadlen = writable_serialized_payload.size();
  pubmsg.qos = QOS;
  pubmsg.retained = 0;

  std::vector<char> writable_mqtt_topic(mqtt_topic.begin(), mqtt_topic.end());

  MQTTClient_publishMessage(client, &writable_mqtt_topic[0], &pubmsg, &token);
  std::cout << "Publishing to MQTT server on "  << mqtt_topic << std::endl;
  std::cout << serialized_payload << std::endl;
  rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
  std::cout << "MQTT message delivered" << std::endl;
  MQTTClient_disconnect(client, 10000);
}

void increment_mqtt() {
  std::string payload = "{\"counter\": \"1\" }";
  log_mqtt(payload);
}

/* MQTT helpers */
std::string random_string()
{
    auto randchar = []() -> char
    {
        const char charset[] =
        "0123456789"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::stringstream rand_str;
    for(int i = 0; i < 16; i++)
    {
      rand_str << randchar();
    }
    return rand_str.str();
}

MqttServiceName map_mqtt_service_name(const std::string &name)
{
  if (name == "m2x")
  {
    return MqttServiceName::M2X;
  }

  return MqttServiceName::None;
}

std::string std_getenv(const std::string& name)
{
  auto value = getenv(name.c_str());
  return value != nullptr ? std::string(value) : std::string();
}
