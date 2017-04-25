#ifndef SERVICES_TRANSPORTS_M2X_M2X_H_INCLUDED
#define SERVICES_TRANSPORTS_M2X_M2X_H_INCLUDED

#include <string>
#include <iostream>
#include <utility>
#include <ctime>

#include "../../../../lib/restclient-cpp/include/restclient-cpp/restclient.h"
#include "../../../../lib/jsoncpp/json/json.h"

#include "../../util.h"

struct m2x_service_config
{
    std::string api_key;
    std::string device_id;
    std::string stream_id;
};

std::pair<m2x_service_config, bool> get_m2x_config();
void m2x_publish(std::string const &message);

#endif