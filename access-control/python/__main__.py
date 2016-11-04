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

from __future__ import print_function, division

from datetime import datetime

# This program is using the Python stand 'importlib' module
# to dynamically import the correct Board class based on config.json
from importlib import import_module

# This program is using the 'simplejson' package
# to serialize and deserialize json data.
from simplejson import load as load_json

# This program is using the 'bottle' package
# to server static html files and JSON data.
from bottle import route, request, run, static_file, HTTPResponse

from alarm import Alarm

from mqtt import publish_message
from storage import store_message

# Load configuration data from `config.json` file. Edit this file
# to change to correct values for your configuration
with open("config.json") as data:
    config = load_json(data)

if "kit" in config:
    print("loading board config \"", config["kit"], "\" from config.json", sep = "")
    Board = getattr(import_module(config["kit"]), config["kit"].capitalize() + "Board")
else:
    print("loading default board config")
    Board = import_module("grove").GroveBoard

board = Board(config)

def log(event):

    """
    Publish data to MQTT server and data store.
    """

    msg = "{0} {1}".format(datetime.utcnow().isostring(), event)

    print(msg)
    payload = { "value": msg }
    publish_message(config, payload)
    store_message(config, payload)

# alarm setup
alarm = Alarm(config, board)

# server setup
alarm_duration = {
    "hour": 0,
    "minute": 0,
    "second": 0
}

@route('/')
def serve_index():

    """
    Serve the 'index.html' file and process alarm query parameters.
    """

    return static_file('index.html', root = "")

@route('/alarm')
def validate_code():

    """
    Handle alarm code validation.
    """

    if { "code" } <= set(request.query):
        code = request.query.get("code")
        validation_result = alarm.validate_code(code)
        
        if validation_result:
            return HTTPResponse(status=200)
        else:
            return HttpResponse(status=403)

    return HTTPResponse(status=400)

def main():

    """
    Start main function.
    """

    run(host = "0.0.0.0", port = 3000)

if __name__ == "__main__":
    main()