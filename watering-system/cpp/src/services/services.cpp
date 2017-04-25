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

    /* Predix Service Dispatch */

    auto predix_config_result = get_predix_config();
    predix_service_config predix_config;
    bool predix_config_valid;
    std::tie(predix_config, predix_config_valid) = predix_config_result;

    if (predix_config_valid) {
    	predix_publish(message);
    }

    /* MQTT Service Dispatch */
    auto mqtt_config_result = get_mqtt_config();
    mqtt_service_config mqtt_config;
    bool mqtt_config_valid;
    std::tie(mqtt_config, mqtt_config_valid) = mqtt_config_result;

    if (mqtt_config_valid) {
    	mqtt_publish(message);
    }

}
