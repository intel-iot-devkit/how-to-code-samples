from __future__ import print_function
from urllib import urlencode
from simplejson import dumps as serialize_json

from twisted.internet.defer import succeed
from twisted.web.iweb import IBodyProducer
from zope.interface import implementer

@implementer(IBodyProducer)
class JsonProducer(object):

    def __init__(self, body):
        data = serialize_json(body)
        self.body = data
        self.length = len(data)

    def startProducing(self, consumer):
        consumer.write(self.body)
        return succeed(None)

    def pauseProducing(self):
        pass

    def stopProducing(self):
        pass

@implementer(IBodyProducer)
class WebFormProducer(object):

    def __init__(self, body):
        data = urlencode(body)
        self.body = data
        self.length = len(data)

    def startProducing(self, consumer):
        consumer.write(self.body)
        return succeed(None)

    def pauseProducing(self):
        pass

    def stopProducing(self):
        pass
