from __future__ import print_function
from datetime import datetime
from uuid import uuid4
from ssl import PROTOCOL_TLSv1
from paho.mqtt.publish import single as mqtt_publish_single
from paho.mqtt.client import MQTTv311
from simplejson import dumps as serialize_json
from ..config import M2X_CONFIG
from ..scheduler import SCHEDULER

m2x_mqtt_hostname = "api-m2x.att.com"
m2x_mqtt_port = 8883

def publish(message):

    if not M2X_CONFIG and not M2X_CONFIG.api_key and not M2X_CONFIG.device_id and not M2X_CONFIG.stream_id:
        return

    m2x_api_key = M2X_CONFIG.api_key
    m2x_device_id = M2X_CONFIG.device_id
    m2x_stream_id = M2X_CONFIG.stream_id

    topic = "m2x/{0}/requests".format(m2x_api_key)
    data = serialize_json({
        "id": str(uuid4()),
        "method": "POST",
        "resource": "/v2/devices/{0}/streams/{1}/values".format(m2x_device_id, m2x_stream_id),
        "body": {
            "values": [
                {
                    "timestamp": datetime.utcnow().isoformat(),
                    "value": serialize_json(message)
                }
            ]
        }
    })

    def perform_request():

        print("Publishing M2X MQTT service.")

        auth = {
            "username": m2x_api_key
        }

        tls = {
            "ca_certs": "/etc/ssl/certs/ca-certificates.crt",
            "tls_version": PROTOCOL_TLSv1
        }

        mqtt_publish_single(
            topic,
            payload=data,
            hostname=m2x_mqtt_hostname,
            port=m2x_mqtt_port,
            auth=auth,
            tls=tls,
            protocol=MQTTv311
        )

        print("Published to M2X MQTT service.")

    SCHEDULER.add_job(perform_request)
