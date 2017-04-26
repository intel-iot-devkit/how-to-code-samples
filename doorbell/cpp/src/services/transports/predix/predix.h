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

#if defined(USE_PREDIX_SERVICE)
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
#endif
