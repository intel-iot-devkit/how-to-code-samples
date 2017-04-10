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

# services

SERVICES = "services"

M2XConfig = namedtuple("M2XConfig", "api_key device_id stream_id")

M2X_SERVICE_NAME = "m2x"
M2X_API_KEY = "api_key"
M2X_DEVICE_ID = "device_id"
M2X_STREAM_ID = "stream_id"

PredixConfig = namedtuple("Predix", "uaa_client_id uaa_client_secret uaa_url timeseries_zone_id timeseries_ingest_url")

PREDIX_SERVICE_NAME = "predix"
PREDIX_UAA_CLIENT_ID = "uaa_client_id"
PREDIX_UAA_CLIENT_SECRET = "uaa_client_secret"
PREDIX_UAA_URL = "uaa_url"
PREDIX_TIMESERIES_ZONE_ID = "timeseries_zone_id"
PREDIX_TIMESERIES_INGEST_URL = "timeseries_ingest_url"

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

    RAW_CONFIG = load_json(data)

    HARDWARE_CONFIG = HardwareConfig(
        kit=RAW_CONFIG.get(KIT, KNOWN_KITS.grove),
        platform=RAW_CONFIG.get(PLATFORM)
    )

    MQTT_CONFIG = MqttConfig(
        server=RAW_CONFIG.get(MQTT_SERVER),
        port=RAW_CONFIG.get(MQTT_PORT, 1883),
        client_id=RAW_CONFIG.get(MQTT_CLIENTID),
        username=RAW_CONFIG.get(MQTT_USERNAME),
        password=RAW_CONFIG.get(MQTT_PASSWORD),
        cert=RAW_CONFIG.get(MQTT_CERT),
        key=RAW_CONFIG.get(MQTT_KEY),
        topic=RAW_CONFIG.get(MQTT_TOPIC),
        service=RAW_CONFIG.get(MQTT_SERVICE)
    ) if {MQTT_SERVER} <= set(RAW_CONFIG) else None

    DATA_STORE_CONFIG = DataStoreConfig(
        server=RAW_CONFIG.get(SERVER),
        auth_token=RAW_CONFIG.get(AUTH_TOKEN)
    ) if {SERVER, AUTH_TOKEN} <= set(RAW_CONFIG) else None
