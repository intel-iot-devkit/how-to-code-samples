from __future__ import print_function
from datetime import datetime
from importlib import import_module
from ..config import M2X_CONFIG, PREDIX_CONFIG, SAP_CONFIG, MQTT_CONFIG
from .m2x import publish as m2x_publish
from .predix import publish as predix_publish
from .sap import publish as sap_publish
from .mqtt import publish as mqtt_publish

# module init

services = []

if M2X_CONFIG:
    services.append(m2x_publish)

if PREDIX_CONFIG:
    services.append(predix_publish)

if SAP_CONFIG:
    services.append(sap_publish)

if MQTT_CONFIG:
    services.append(mqtt_publish)

# module api

def service_message(event):

    for service_publish in services:
        service_publish(event)
