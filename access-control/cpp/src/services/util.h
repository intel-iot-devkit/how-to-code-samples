#ifndef SERVICES_UTIL_H_INCLUDED
#define SERVICES_UTIL_H_INCLUDED

#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>

#include <string>
#include <sstream>

std::string random_string();
std::string std_getenv(const std::string &name);

typedef websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> ssl_context_ptr;
ssl_context_ptr tls_init(websocketpp::connection_hdl);

#endif