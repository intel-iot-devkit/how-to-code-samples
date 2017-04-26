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
#include "predix.h"

ws_client wsc;
messages_list messages;
websocketpp::lib::shared_ptr<websocketpp::lib::thread> network_thread;

bool client_init = false;
bool ws_init = false;

std::pair<predix_service_config, bool> get_predix_config()
{
    const auto predix_uaa_client_id = std_getenv("PREDIX_UAA_CLIENT_ID");
    const auto predix_uaa_client_secret = std_getenv("PREDIX_UAA_CLIENT_SECRET");
    const auto predix_uaa_url = std_getenv("PREDIX_UAA_URL");
    const auto predix_timeseries_zone_id = std_getenv("PREDIX_TIMESERIES_ZONE_ID");
    const auto predix_timeseries_ingest_url = std_getenv("PREDIX_TIMESERIES_INGEST_URL");

    auto config_valid = false;

    predix_service_config config = {
        predix_uaa_client_id,
        predix_uaa_client_secret,
        predix_uaa_url,
        predix_timeseries_zone_id,
        predix_timeseries_ingest_url
    };

    if (!predix_uaa_client_id.empty() && !predix_uaa_client_secret.empty() && !predix_uaa_url.empty() &&
        !predix_timeseries_zone_id.empty() && !predix_timeseries_ingest_url.empty())
    {
        config_valid = true;
    }

    std::pair<predix_service_config, bool> result {
        config,
        config_valid
    };

    return result;
}

std::string uaa_authenticate(std::string const &uaa_endpoint, std::string const &client_id, std::string const &client_secret)
{
    std::stringstream predix_token_endpoint_stream;
    predix_token_endpoint_stream << uaa_endpoint << "/oauth/token";
    auto predix_token_endpoint = predix_token_endpoint_stream.str();

    std::stringstream payload_stream;
    payload_stream
        << "grant_type=client_credentials" << "&"
        << "client_id=" << client_id << "&"
        << "client_secret=" << client_secret << "&"
        << "response_type=token";
    auto payload = payload_stream.str();

    RestClient::response response = RestClient::post(predix_token_endpoint, "application/x-www-form-urlencoded", payload);
    
    std::string token;
    if (response.code == 200) {
        Json::Value auth;
        Json::Reader reader;
        auto json_result = reader.parse(response.body, auth);
        if (json_result) {
            token = auth["access_token"].asString();
            std::cout << "UAA Access Token: " << token << std::endl;
        }
    }

    return token;
}

void timeseries_push(std::string const &ingest_endpoint, std::string const &zone_id, std::string const &token, std::string const &message)
{
    // timeseries payload
    Json::Value data;
    data["messageId"] = random_string();
        Json::Value body_values(Json::arrayValue);
        Json::Value body_value;
        body_value["name"] = "sensor-data";
        Json::Value sensor_values(Json::arrayValue);
            Json::Value sensor_value(Json::arrayValue);
            auto timestamp = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
            sensor_value.append(timestamp);
            sensor_value.append(message);
        sensor_values.append(sensor_value);
        body_value["datapoints"] = sensor_values;
        body_values.append(body_value);
    data["body"] = body_values;

    Json::FastWriter writer;
    auto payload = writer.write(data);
    std::cout << "Predix Payload: " << payload;

    // timeseries push
    setup_websocket();
    setup_connection(ingest_endpoint, zone_id, token, payload);
}

void on_open(ws_client *c, websocketpp::connection_hdl hdl)
{
    std::string data = messages.find(hdl)->second;
    websocketpp::lib::error_code ec;
    c->send(hdl, data, websocketpp::frame::opcode::text, ec);

    if (ec)
    {
        std::cout << "Error sending Predix message: " << ec.message() << std::endl;
    }
    else
    {
        std::cout << "Predix WebSocket open." << std::endl;
    }
}

void on_fail(ws_client *c, websocketpp::connection_hdl hdl)
{
    messages.erase(hdl);
    auto con = wsc.get_con_from_hdl(hdl);
    std::cout << "Predix connection failed: " << con->get_ec().message() << std::endl;
}

void on_message(ws_client *c, websocketpp::connection_hdl hdl, ws_message_ptr msg)
{
    if (msg->get_opcode() == websocketpp::frame::opcode::text)
    {
        std::cout << "Predix response: " << msg->get_payload() << std::endl;
    }
}

void on_close(ws_client *c, websocketpp::connection_hdl hdl)
{
    messages.erase(hdl);
    std::cout << "Predix connection closed." << std::endl;
}

void setup_websocket()
{
    if (!client_init)
    {
        wsc.clear_access_channels(websocketpp::log::alevel::all);
        wsc.clear_error_channels(websocketpp::log::elevel::all);
        wsc.init_asio();
        wsc.start_perpetual();
        wsc.set_tls_init_handler(websocketpp::lib::bind(&tls_init, websocketpp::lib::placeholders::_1));
        network_thread.reset(new websocketpp::lib::thread(&ws_client::run, &wsc));
        client_init = true;
    }
}

void setup_connection(std::string const &ws_url, std::string const &zone_id, std::string const &access_token, std::string const &payload)
{
    websocketpp::lib::error_code ec;
    ws_client::connection_ptr con = wsc.get_connection(ws_url, ec);

    if (ec)
    {
        std::cout << "Predix connect initialization error: " << ec.message() << std::endl;
        return;
    }

    std::cout << "Predix connection established." << std::endl;

    con->replace_header("Origin", "https://intel-how-to-samples.example.org");
    con->replace_header("Predix-Zone-Id", zone_id);
    con->replace_header("Authorization", "Bearer " + access_token);

    con->set_open_handler(websocketpp::lib::bind(&on_open, &wsc, websocketpp::lib::placeholders::_1));
    con->set_fail_handler(websocketpp::lib::bind(&on_fail, &wsc, websocketpp::lib::placeholders::_1));
    con->set_message_handler(websocketpp::lib::bind(&on_message, &wsc, websocketpp::lib::placeholders::_1, websocketpp::lib::placeholders::_2));
    con->set_close_handler(websocketpp::lib::bind(&on_close, &wsc, websocketpp::lib::placeholders::_1));

    auto handle = con->get_handle();
    messages[handle] = payload;
    wsc.connect(con);
}

void predix_publish(std::string const &message)
{
    auto predix_config_result = get_predix_config();

    predix_service_config predix_config;
    bool predix_config_valid;

    std::tie(predix_config, predix_config_valid) = predix_config_result;

    if (!predix_config_valid)
    {
        std::cout << "No Predix config environment variables." << std::endl;
        return;
    }

    auto token = uaa_authenticate(predix_config.uaa_url, predix_config.uaa_client_id, predix_config.uaa_client_secret);

    if (token.empty()) {
        std::cout << "Predix authentication failed. Check Predix config." << std::endl;
        return;
    }

    timeseries_push(predix_config.timeseries_ingest_url, predix_config.timeseries_zone_id, token, message);
}

#endif