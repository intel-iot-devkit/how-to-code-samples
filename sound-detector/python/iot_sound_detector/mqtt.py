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

from __future__ import print_function
from ssl import PROTOCOL_TLSv1
from paho.mqtt.publish import single as mqtt_publish_single
from paho.mqtt.client import MQTTv311
from simplejson import dumps as serialize_json
from .config import MQTT_CONFIG
from .scheduler import SCHEDULER

def publish_message(payload):

    """
    Publish message to MQTT server.
    """

    if not MQTT_CONFIG:
        return

    server = MQTT_CONFIG.server
    port = 1883 if MQTT_CONFIG.port is None else MQTT_CONFIG.port

    client_id = MQTT_CONFIG.client_id

    auth = {
        "username": MQTT_CONFIG.username,
        "password": MQTT_CONFIG.password
    }

    tls = {
        "ca_certs": "/etc/ssl/certs/ca-certificates.crt",
        "tls_version": PROTOCOL_TLSv1,
        "certfile": MQTT_CONFIG.cert,
        "keyfile": MQTT_CONFIG.key
    }

    topic = MQTT_CONFIG.topic
    data = serialize_json(payload)

    def perform_request():

        """
        Perform MQTT request.
        """

        mqtt_publish_single(
            topic, payload=data,
            hostname=server,
            port=port,
            client_id=client_id,
            auth=auth,
            tls=tls,
            protocol=MQTTv311
        )

        print("published to MQTT server")

    SCHEDULER.add_job(perform_request)
