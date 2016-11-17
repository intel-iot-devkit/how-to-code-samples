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

from ssl import PROTOCOL_TLSv1

from paho.mqtt.publish import single as mqtt_publish_single
from paho.mqtt.client import MQTTv311
from simplejson import dumps as serialize_json

from constants.config import MQTT_SERVER, MQTT_PORT, MQTT_CLIENTID, MQTT_USERNAME, MQTT_PASSWORD, MQTT_CERT, MQTT_KEY, MQTT_TOPIC

from scheduler import scheduler

def publish_message(config, payload):

    """
    Publish message to MQTT server.
    """

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

    def perform_request():
        mqtt_publish_single(topic, payload=data, hostname=server, port=port, client_id=client_id, auth=auth, tls=tls, protocol=MQTTv311)
        print("published to MQTT server")

    scheduler.add_job(perform_request)
