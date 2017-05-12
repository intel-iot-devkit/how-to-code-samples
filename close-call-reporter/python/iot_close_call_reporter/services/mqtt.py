from __future__ import print_function
from datetime import datetime
from uuid import uuid4
from ssl import PROTOCOL_TLSv1
from paho.mqtt.publish import single as mqtt_publish_single
from paho.mqtt.client import MQTTv311
from simplejson import dumps as serialize_json
from ..config import MQTT_CONFIG
from ..scheduler import SCHEDULER

def publish(message):

    if not MQTT_CONFIG:
        return

    server = MQTT_CONFIG.server
    port = MQTT_CONFIG.port

    client_id = MQTT_CONFIG.client_id

    auth = {
        "username": MQTT_CONFIG.username,
        "password": MQTT_CONFIG.password
    } if MQTT_CONFIG.username and MQTT_CONFIG.password else None

    tls = {
        "ca_certs": "/etc/ssl/certs/ca-certificates.crt",
        "tls_version": PROTOCOL_TLSv1,
        "certfile": MQTT_CONFIG.cert,
        "keyfile": MQTT_CONFIG.key
    } if MQTT_CONFIG.port == 8883 else None

    topic = MQTT_CONFIG.topic
    data = message

    def perform_request():

        print("Publishing to MQTT service:", data)

        mqtt_publish_single(
            topic,
            payload=data,
            hostname=server,
            port=port,
            client_id=client_id,
            auth=auth,
            tls=tls,
            protocol=MQTTv311
        )

        print("Published to MQTT service.")

    SCHEDULER.add_job(perform_request)
