
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

void log_mqtt(std::string payload);

#endif /* MQTT_H_ */
