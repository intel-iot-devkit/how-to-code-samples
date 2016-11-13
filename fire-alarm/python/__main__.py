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

# This program is using the Python stand 'importlib' module
# to dynamically import the correct Board class based on config.json
from importlib import import_module

# This program is using the 'simplejson' package
# to serialize and deserialize json data.
from simplejson import load as load_json

from events import scheduler, emitter, ms
from twilioo import send_sms
from mqtt import publish_message
from storage import store_message
from time import sleep

prev = 0
current = 0
alarm_interval = ""


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

def monitorTemperature():

    """
    Read temperature sensor in Celsius.
    """
    global current
    global prev

    current = board.get_temperature()
    board.write_message("TEMPERATURE: " + str(current))

    if(prev < config["ALARM_THRESHOLD"] and current >= config["ALARM_THRESHOLD"]):
        start_alarm()

    if(prev >= config["ALARM_THRESHOLD"] and current < config["ALARM_THRESHOLD"]):
        stop_alarm()

    prev = current 

def start_alarm():
    
    global current
    global alarm_interval

    print("alarm started")

    board.start_buzzer()
    board.change_background("red")

    alarm_state = {
        "tick": False
    }

    def alarm_actions():

        """
        Perform alarm actions.
        Alternates LCD background color and buzzer sound.
        """

        tick = alarm_state["tick"]
        board.change_background("white" if tick else "red")
        alarm_state["tick"] = not tick
    
    alarm_interval = scheduler.add_job(alarm_actions, "interval", seconds=ms(250))

    def notify(temperature):

        """
        Publish data to MQTT server and data store.
        """

        payload = { "value": temperature }
        sms = "fire alarm! " + str(temperature)
        publish_message(config, payload)
        store_message(config, payload)
        send_sms(config, sms)

    notify(current)

def stop_alarm():

    print("alarm stopped")

    global alarm_interval

    alarm_interval.remove()
    board.change_background("white")
    board.stop_buzzer()

def main():

    """
    Start main function.
    """

    board.stop_buzzer()

    while True:
        monitorTemperature()
        sleep(5)

if __name__ == "__main__":
    main()
