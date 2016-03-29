
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
     printf("Failed to connect to MQTT server, return code %d\n", rc);
     return;
  }
  pubmsg.payload = &payload;
  pubmsg.payloadlen = payload.length();
  pubmsg.qos = QOS;
  pubmsg.retained = 0;
  MQTTClient_publishMessage(client, getenv("MQTT_TOPIC"), &pubmsg, &token);
  printf("Waiting for up to %d seconds for publication of %s\n"
         "on topic %s for client with ClientID: %s\n",
         (int)(TIMEOUT/1000), &payload[0], getenv("MQTT_TOPIC"), getenv("MQTT_CLIENTID"));
  rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
  printf("MQTT Message with delivery token %d delivered\n", token);
  MQTTClient_disconnect(client, 10000);
  MQTTClient_destroy(&client);
}


