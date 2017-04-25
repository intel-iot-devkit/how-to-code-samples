#ifndef SERVICES_TRANSPORTS_PREDIX_PREDIX_H_INCLUDED
#define SERVICES_TRANSPORTS_PREDIX_PREDIX_H_INCLUDED

#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>

#include <websocketpp/common/thread.hpp>
#include <websocketpp/common/memory.hpp>

#include <string>
#include <chrono>
#include <map>
#include <iostream>
#include <utility>

#include "../../../../lib/restclient-cpp/include/restclient-cpp/restclient.h"
#include "../../../../lib/jsoncpp/json/json.h"

#include "../../util.h"

struct predix_service_config
{
    std::string uaa_client_id;
    std::string uaa_client_secret;
    std::string uaa_url;
    std::string timeseries_zone_id;
    std::string timeseries_ingest_url;
};

typedef std::map<websocketpp::connection_hdl,std::string,std::owner_less<websocketpp::connection_hdl>> messages_list;

typedef websocketpp::client<websocketpp::config::asio_tls_client> ws_client;
typedef websocketpp::config::asio_client::message_type::ptr ws_message_ptr;

std::string uaa_authenticate(std::string const &uaa_endpoint, std::string const &client_id, std::string const &client_secret);
void timeseries_push(std::string const &ingest_endpoint, std::string const &zone_id, std::string const &token, std::string const &message);

void setup_websocket();
void setup_connection(std::string const &ws_url, std::string const &zone_id, std::string const &access_token, std::string const &payload);

std::pair<predix_service_config, bool> get_predix_config();
void predix_publish(std::string const &message);

#endif
