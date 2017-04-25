#ifndef SERVICES_TRANSPORTS_MQTT_MQTT_H_INCLUDED
#define SERVICES_TRANSPORTS_MQTT_MQTT_H_INCLUDED

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>

#include "../../util.h"

extern "C" {
    #include "MQTTClient.h"
    #include "MQTTClientPersistence.h"
}

#define QOS 1
#define TIMEOUT 10000L

struct mqtt_service_config
{
    std::string server;
    std::string client_id;
    std::string topic;
    std::string username;
    std::string password;
    std::string cert;
    std::string cert_key;
    std::string ca_root;
};

std::pair<mqtt_service_config, bool> get_mqtt_config();
void mqtt_publish(std::string const &message);

#endif