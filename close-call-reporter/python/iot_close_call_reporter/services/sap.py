from __future__ import print_function
from simplejson import dumps as serialize_json
from time import time

from twisted.internet import reactor, ssl
from twisted.web.client import Agent, BrowserLikeRedirectAgent, readBody
from twisted.web.http_headers import Headers

from .utils import JsonProducer
from ..config import SAP_CONFIG

def push(message):

    sap_mms_endpoint = SAP_CONFIG.mms_endpoint
    sap_device_id = SAP_CONFIG.device_id
    sap_message_type_id = SAP_CONFIG.message_type_id
    sap_oauth_token = SAP_CONFIG.oauth_token

    sap_data_endpoint = "{0}/{1}".format(sap_mms_endpoint, sap_device_id)

    print("SAP MMS data endpoint: {0}".format(sap_data_endpoint))

    agent = Agent(reactor)
    client = BrowserLikeRedirectAgent(agent)

    headers = Headers({
        "Authorization": ["Bearer {0}".format(sap_oauth_token)],
        "Content-Type": ["application/json;charset=utf-8"]
    })

    body = {
        "mode": "sync",
        "messageType": sap_message_type_id,
        "messages": [{
            "sensor": "IntelHowToSample",
            "value": serialize_json(message),
            "timestamp": int(time())
        }]
    }

    print("SAP MMS payload: {0}".format(serialize_json(body)))

    def handle_response(response):
        if response.code == 200:
            print("SAP MMS publish succeeded.")
        else:
            print("SAP Error response: {0}".format(response.code))

    client.request("POST", sap_data_endpoint, headers, JsonProducer(body)) \
        .addCallback(lambda x: print("Published to SAP MMS service.")).addErrback(lambda x: print("SAP publish error:", x))

def publish(message):

    print("Publishing to SAP MMS service.")
    push(message)

