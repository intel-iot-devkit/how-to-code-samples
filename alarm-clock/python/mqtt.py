from ssl import PROTOCOL_TLSv1

from paho.mqtt.publish import single as mqtt_publish_single
from paho.mqtt.client import MQTTv311
from simplejson import dumps as serialize_json

# constants

MQTT_SERVER = "MQTT_SERVER"
MQTT_PORT = "MQTT_PORT"
MQTT_CLIENTID = "MQTT_CLIENTID"
MQTT_USERNAME = "MQTT_USERNAME"
MQTT_PASSWORD = "MQTT_PASSWORD"
MQTT_CERT = "MQTT_CERT"
MQTT_KEY = "MQTT_KEY"
MQTT_TOPIC = "MQTT_TOPIC"

def publish_message(config, payload):

    if not { MQTT_SERVER, MQTT_CLIENTID, MQTT_TOPIC } <= set(config): return

    server = config[MQTT_SERVER]
    port = config.get(MQTT_PORT, 1883)

    client_id = config[MQTT_CLIENTID]

    auth = {
        "username": config.get(MQTT_USERNAME),
        "password": config.get(MQTT_PASSWORD)
    }

    tls = {
        "ca_certs": "/etc/ssl/certs/ca-certificates.crt",
        "tls_version": PROTOCOL_TLSv1,
        "certfile": config.get(MQTT_CERT),
        "keyfile": config.get(MQTT_KEY)
    }

    topic = config[MQTT_TOPIC]
    data = serialize_json(payload)

    mqtt_publish_single(topic, payload=data, hostname=server, port=port, client_id=client_id, auth=auth, tls=tls, protocol=MQTTv311)
