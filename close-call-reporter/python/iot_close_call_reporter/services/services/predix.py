from __future__ import print_function
from uuid import uuid4
from time import time
from simplejson import dumps as serialize_json, loads as deserialize_json

from twisted.internet import reactor, ssl
from twisted.web.client import Agent, BrowserLikeRedirectAgent, readBody
from twisted.web.http_headers import Headers
from autobahn.twisted.websocket import WebSocketClientFactory, WebSocketClientProtocol, connectWS

from .utils import WebFormProducer
from ..config import PREDIX_CONFIG

def auth():

    predix_client_id = PREDIX_CONFIG.uaa_client_id
    predix_client_secret = PREDIX_CONFIG.uaa_client_secret
    predix_uaa_endpoint = "{0}/oauth/token".format(PREDIX_CONFIG.uaa_url)

    agent = Agent(reactor)
    client = BrowserLikeRedirectAgent(agent)

    headers = Headers({
        "Content-Type": ["application/x-www-form-urlencoded"]
    })

    body = WebFormProducer({
        "grant_type": "client_credentials",
        "client_id": predix_client_id,
        "client_secret": predix_client_secret,
        "response_type": "token"
    })

    def handle_response(response):
        if response.code != 200:
            raise IOError("Predix UAA token request failed. Check Predix config values.")
        return readBody(response)

    def handle_body(body):
        data = deserialize_json(body)
        token = data["access_token"]
        return token

    return client \
        .request("POST", predix_uaa_endpoint, headers, body) \
        .addCallback(handle_response) \
        .addCallback(handle_body)

def push(token, data):

    predix_ingestion_endpoint = PREDIX_CONFIG.timeseries_ingest_url
    predix_zone_id = PREDIX_CONFIG.timeseries_zone_id

    client = PredixTimeSeriesClient(predix_ingestion_endpoint, predix_zone_id, token)
    client.pushDatapoint(data)

def publish(message):

    print("Publishing to Predix Timeseries service.")

    auth() \
        .addCallback(lambda token: push(token, message)).addErrback(lambda x: print("Predix auth error:", x)) \
        .addCallback(lambda x: print("Published to Predix Timeseries service.")).addErrback(lambda x: print("Predix push error:", x))

class PredixTimeSeriesClient(object):

    def __init__(self, endpoint, zone_id, token):

        self.endpoint = endpoint
        self.origin = "https://intel-how-to-samples.example.org"

        self.headers = {
            "Predix-Zone-Id": zone_id,
            "Authorization": "Bearer {0}".format(token)
        }

    def pushDatapoint(self, data):

        factory = PredixTimeSeriesFactory(self.endpoint, origin=self.origin, headers=self.headers, payload=data)
        factory.protocol = PredixTimeSeriesProtocol

        ssl_ctx = ssl.ClientContextFactory()
        connectWS(factory, ssl_ctx)

class PredixTimeSeriesFactory(WebSocketClientFactory):

    def __init__(self, endpoint, origin, headers, payload):

        super(PredixTimeSeriesFactory, self).__init__(endpoint, origin=origin, headers=headers)
        self.payload = payload

class PredixTimeSeriesProtocol(WebSocketClientProtocol):

    def __init__(self):

        super(PredixTimeSeriesProtocol, self).__init__()
        self.message_id = str(uuid4())

    def onConnect(self, response):
        print(response)

    def onOpen(self):

        print("Connected to Predix Time Series service.")
        self.pushDatapoint()

    def onClose(self, clean, code, reason):

        print("Predix Time Series connetion closed.")

    def pushDatapoint(self):

        data = {
            "messageId": self.message_id,
            "body": [{
                "name": "sensor-data",
                "datapoints": [
                    [
                        int(time() * 1000),
                        serialize_json(self.factory.payload, ensure_ascii=False)
                    ]
                ]
            }]
        }

        serialized_data = serialize_json(data, ensure_ascii=False).encode("utf-8")
        self.sendMessage(serialized_data)

    def onMessage(self, payload, isBinary):

        response = deserialize_json(payload.decode("utf-8"))
        msg_id = response.get("messageId")
        status_code = response.get("statusCode")
        if msg_id:
            print("Predix response: msg_id({0}) status:{1}".format(msg_id, status_code))
        self.sendClose()
