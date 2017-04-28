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

#include "services.h"

void increment_service() {
    std::string payload = "{\"counter\": \"1\" }";
    log_service(payload);
}

void log_service(std::string const &message) {
    
    /* M2X Service Dispatch */
    
    auto m2x_config_result = get_m2x_config();
    m2x_service_config m2x_config;
    bool m2x_config_valid;
    std::tie(m2x_config, m2x_config_valid) = m2x_config_result;

    if (m2x_config_valid) {
        m2x_publish(message);
    }

    /* SAP Service Dispatch */
    
    auto sap_config_result = get_sap_config();
    sap_service_config sap_config;
    bool sap_config_valid;
    std::tie(sap_config, sap_config_valid) = sap_config_result;

    if (sap_config_valid) {
        sap_publish(message);
    }

    /* Predix Service Dispatch */

	#if defined(USE_PREDIX_SERVICE)
    auto predix_config_result = get_predix_config();
    predix_service_config predix_config;
    bool predix_config_valid;
    std::tie(predix_config, predix_config_valid) = predix_config_result;

    if (predix_config_valid) {
    	predix_publish(message);
    }
	#endif

    /* MQTT Service Dispatch */
    auto mqtt_config_result = get_mqtt_config();
    mqtt_service_config mqtt_config;
    bool mqtt_config_valid;
    std::tie(mqtt_config, mqtt_config_valid) = mqtt_config_result;

    if (mqtt_config_valid) {
    	mqtt_publish(message);
    }
}
