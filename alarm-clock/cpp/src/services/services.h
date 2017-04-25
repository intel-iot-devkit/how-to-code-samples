#ifndef SERVICES_SERVICES_H_INCLUDED
#define SERVICES_SERVICES_H_INCLUDED

#include <string>
#include <vector>
#include <iostream>
#include <utility>

#include "transports/m2x/m2x.h"
#include "transports/predix/predix.h"
#include "transports/mqtt/mqtt.h"

void increment_service();
void log_service(std::string const &message);

#endif
