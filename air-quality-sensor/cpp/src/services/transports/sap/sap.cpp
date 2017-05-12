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

#include "sap.h"

std::pair<sap_service_config, bool> get_sap_config()
{
    const auto sap_api_key = std_getenv("SAP_MMS_ENDPOINT");
    const auto sap_device_id = std_getenv("SAP_DEVICE_ID");
    const auto sap_message_type_id= std_getenv("SAP_MESSAGE_TYPE_ID");
    const auto sap_oauth_token = std_getenv("SAP_OAUTH_TOKEN");

    auto config_valid = false;

    sap_service_config config = {
        sap_api_key,
        sap_device_id,
        sap_message_type_id,
        sap_oauth_token
    };

    if (!sap_api_key.empty() && !sap_device_id.empty() && !sap_message_type_id.empty() && !sap_oauth_token.empty())
    {
        config_valid = true;
    }

    std::pair<sap_service_config, bool> result = {
        config,
        config_valid};

    return result;
}

void sap_publish(std::string const &message)
{
    auto sap_config_result = get_sap_config();

    sap_service_config sap_config;
    bool sap_config_valid;

    std::tie(sap_config, sap_config_valid) = sap_config_result;

    if (!sap_config_valid)
    {
        std::cout << "No sap config environment variables." << std::endl;
        return;
    }

    // sap http request
    std::stringstream endpoint_buffer;
    endpoint_buffer << sap_config.mms_endpoint << "/" << sap_config.device_id;
    auto sap_data_endpoint = endpoint_buffer.str();

    std::cout << "SAP MMS data endpoint: " << sap_data_endpoint << std::endl;

    // sap data payload
    Json::Value root;

    root["mode"] = "sync";
    root["messageType"] = sap_config.message_type_id;
    Json::Value messages(Json::arrayValue);
        Json::Value msg;
        msg["sensor"] = "IntelHowToSample";
        msg["value"] = message;
        time_t now;
        time(&now);
        std::stringstream timebuf;
        timebuf << now;
        msg["timestamp"] = timebuf.str();
        messages.append(msg);
    root["messages"] = messages;

    Json::FastWriter writer;
    auto serialized_payload = writer.write(root);

    std::cout << "SAP MMS payload: " << serialized_payload;

    RestClient::headermap headers;
    std::stringstream auth_buffer;
    auth_buffer << "Bearer " << sap_config.oauth_token;
    headers["Authorization"] = auth_buffer.str();

    RestClient::response response = RestClient::post(sap_data_endpoint, "application/json;charset=utf-8", serialized_payload, headers);
    std::cout << "SAP Service Publish Result: " << response.code << std::endl;
}
