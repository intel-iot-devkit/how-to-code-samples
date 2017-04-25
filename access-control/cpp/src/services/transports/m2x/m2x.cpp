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

#include "m2x.h"

std::pair<m2x_service_config, bool> get_m2x_config()
{
    const auto m2x_api_key = std_getenv("M2X_API_KEY");
    const auto m2x_device_id = std_getenv("M2X_DEVICE_ID");
    const auto m2x_stream_id = std_getenv("M2X_STREAM_ID");

    auto config_valid = false;

    m2x_service_config config = {
        m2x_api_key,
        m2x_device_id,
        m2x_stream_id
    };

    if (!m2x_api_key.empty() && !m2x_device_id.empty() && !m2x_stream_id.empty())
    {
        config_valid = true;
    }

    std::pair<m2x_service_config, bool> result = {
        config,
        config_valid};

    return result;
}

void m2x_publish(std::string const &message)
{
    auto m2x_config_result = get_m2x_config();

    m2x_service_config m2x_config;
    bool m2x_config_valid;

    std::tie(m2x_config, m2x_config_valid) = m2x_config_result;

    if (!m2x_config_valid)
    {
        std::cout << "No M2X config environment variables." << std::endl;
        return;
    }

    Json::Value root;

    // m2x data payload
    Json::Value body_values(Json::arrayValue);
    Json::Value body_value;
    time_t now;
    time(&now);
    char time_buf[sizeof "2017-02-12T07:07:07Z"];
    strftime(time_buf, sizeof time_buf, "%FT%TZ", gmtime(&now));
    body_value["timestamp"] = time_buf;
    body_value["value"] = message;
    body_values.append(body_value);
    root["values"] = body_values;

    Json::FastWriter writer;
    auto serialized_payload = writer.write(root);

    std::cout << "M2X payload: " << serialized_payload << std::endl;

    // m2x http request
    auto m2x_base_endpoint = "https://api-m2x.att.com/v2";

    std::stringstream m2x_stream_endpoint_stream;
    m2x_stream_endpoint_stream <<m2x_base_endpoint << "/devices/" << m2x_config.device_id << "/streams/" << m2x_config.stream_id << "/values";
    std::string m2x_stream_endpoint = m2x_stream_endpoint_stream.str();

    RestClient::headermap headers;
    headers["X-M2X-KEY"] = m2x_config.api_key;
    //headers["Content-Type"] = "application/json";

    RestClient::response response = RestClient::post(m2x_stream_endpoint, "application/json", serialized_payload, headers);
    std::cout << "M2X Service Publish Result: " << response.code << std::endl;
}
