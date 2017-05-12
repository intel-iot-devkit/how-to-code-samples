# Copyright (c) 2015 - 2016 Intel Corporation.
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
# LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
# OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
# WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

from collections import namedtuple
from pkg_resources import resource_stream
from simplejson import load as load_json

# hardware platform
HardwareConfig = namedtuple("HardwareConfig", "kit platform")

KIT = "kit"
PLATFORM = "PLATFORM"

Kits = namedtuple("Kits", "grove dfrobot")
KNOWN_KITS = Kits(
    grove="grove",
    dfrobot="dfrobot"
)

Platforms = namedtuple("Platforms", "firmata")
KNOWN_PLATFORMS = Platforms(
    firmata="firmata"
)

# app specific
AppConfig = namedtuple("AppConfig", "code")

CODE = "CODE"

# services

SERVICES = "services"

M2XConfig = namedtuple("M2XConfig", "api_key device_id stream_id")

M2X_SERVICE_NAME = "m2x"
M2X_API_KEY = "api_key"
M2X_DEVICE_ID = "device_id"
M2X_STREAM_ID = "stream_id"

PredixConfig = namedtuple("PredixConfig", "uaa_client_id uaa_client_secret uaa_url timeseries_zone_id timeseries_ingest_url")

PREDIX_SERVICE_NAME = "predix"
PREDIX_UAA_CLIENT_ID = "uaa_client_id"
PREDIX_UAA_CLIENT_SECRET = "uaa_client_secret"
PREDIX_UAA_URL = "uaa_url"
PREDIX_TIMESERIES_ZONE_ID = "timeseries_zone_id"
PREDIX_TIMESERIES_INGEST_URL = "timeseries_ingest_url"

SapConfig = namedtuple("SapConfig", "mms_endpoint device_id message_type_id oauth_token")

SAP_SERVICE_NAME = "sap"
SAP_MMS_ENDPOINT = "mms_endpoint"
SAP_DEVICE_ID = "device_id"
SAP_MESSAGE_TYPE_ID = "message_type_id"
SAP_OAUTH_TOKEN = "oauth_token"

MqttConfig = namedtuple("MqttConfig", "server port client_id username password cert key topic")

MQTT_SERVICE_NAME = "mqtt"
MQTT_SERVER = "server"
MQTT_PORT = "port"
MQTT_CLIENTID = "client_id"
MQTT_USERNAME = "username"
MQTT_PASSWORD = "password"
MQTT_CERT = "cert"
MQTT_KEY = "key"
MQTT_TOPIC = "topic"

# remote data store
DataStoreConfig = namedtuple("DataStoreConfig", "server auth_token")

SERVER = "SERVER"
AUTH_TOKEN = "AUTH_TOKEN"

# Load configuration data from `project/config.json` file.
# Edit this file to change to correct values for your configuration

RESOURCE_PACKAGE = __name__
RESOURCE_PATH = "config.json"


with resource_stream(RESOURCE_PACKAGE, RESOURCE_PATH) as data:

    raw_config = load_json(data)

    HARDWARE_CONFIG = HardwareConfig(
        kit=raw_config.get(KIT, KNOWN_KITS.grove),
        platform=raw_config.get(PLATFORM)
    )

    APP_CONFIG = AppConfig(
        code=raw_config.get(CODE, "1234")
    )

    DATA_STORE_CONFIG = DataStoreConfig(
        server=raw_config.get(SERVER),
        auth_token=raw_config.get(AUTH_TOKEN)
    ) if {SERVER, AUTH_TOKEN} <= set(raw_config) else None

    # service configs

    raw_services = raw_config.get("services", {})

    raw_predix = raw_services.get(PREDIX_SERVICE_NAME, {})
    PREDIX_CONFIG = PredixConfig(
        uaa_client_id=raw_predix.get(PREDIX_UAA_CLIENT_ID),
        uaa_client_secret=raw_predix.get(PREDIX_UAA_CLIENT_SECRET),
        uaa_url=raw_predix.get(PREDIX_UAA_URL),
        timeseries_zone_id=raw_predix.get(PREDIX_TIMESERIES_ZONE_ID),
        timeseries_ingest_url=raw_predix.get(PREDIX_TIMESERIES_INGEST_URL)
    ) if {
        PREDIX_UAA_CLIENT_ID, PREDIX_UAA_CLIENT_SECRET, PREDIX_UAA_URL,
        PREDIX_TIMESERIES_ZONE_ID, PREDIX_TIMESERIES_INGEST_URL
    } <= set(raw_predix) else None

    raw_m2x = raw_services.get(M2X_SERVICE_NAME, {})
    M2X_CONFIG = M2XConfig(
        api_key=raw_m2x.get(M2X_API_KEY),
        device_id=raw_m2x.get(M2X_DEVICE_ID),
        stream_id=raw_m2x.get(M2X_STREAM_ID)
    ) if {M2X_API_KEY, M2X_DEVICE_ID, M2X_STREAM_ID} <= set(raw_m2x) else None

    raw_sap = raw_services.get(SAP_SERVICE_NAME, {})
    SAP_CONFIG = SapConfig(
        mms_endpoint=raw_sap.get(SAP_MMS_ENDPOINT),
        device_id=raw_sap.get(SAP_DEVICE_ID),
        message_type_id=raw_sap.get(SAP_MESSAGE_TYPE_ID),
        oauth_token=raw_sap.get(SAP_OAUTH_TOKEN)
    ) if {SAP_MMS_ENDPOINT, SAP_OAUTH_TOKEN} <= set(raw_sap) else None

    raw_mqtt = raw_services.get(MQTT_SERVICE_NAME, {})
    MQTT_CONFIG = MqttConfig(
        server=raw_mqtt.get(MQTT_SERVER),
        port=raw_mqtt.get(MQTT_PORT, 1883),
        client_id=raw_mqtt.get(MQTT_CLIENTID),
        username=raw_mqtt.get(MQTT_USERNAME),
        password=raw_mqtt.get(MQTT_PASSWORD),
        cert=raw_mqtt.get(MQTT_CERT),
        key=raw_mqtt.get(MQTT_KEY),
        topic=raw_mqtt.get(MQTT_TOPIC)
    ) if {MQTT_SERVER, MQTT_CLIENTID, MQTT_TOPIC} <= set(raw_mqtt) else None
