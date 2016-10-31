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

# This program is using the 'bottle' package
# to server static html files and JSON data.
from bottle import route, request, run, static_file

# This program is using the 'arrow' package for easier time-based calculations
# to determine when the alarm should sound
from arrow import utcnow

from events import scheduler, emitter, ms
from weather import get_weather
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

# clock functions
current_time = utcnow().replace(seconds=-1)
alarm_time = utcnow().replace(days=-1)

def after(t, c):
    return t.floor("second") > c.floor("second")

def same(t, c):
    return t.floor("second") == c.floor("second")

def start_clock():

    """
    Start clock timer.
    Checks every 50ms to update time and to start the alarm.
    """

    global current_time
    time = utcnow()
    if after(time, current_time):
        board.write_message(str(time.format("h:mm:ss A")))
        if same(time, alarm_time):
            start_alarm()

    current_time = time

scheduler.add_job(start_clock, "interval", coalesce=True, seconds=ms(50))

def start_alarm():

    """
    Start the alarm timer.
    """

    global alarm_time

    alarm_state = {
        "tick": False
    }

    board.start_buzzer()
    board.change_background("red")

    try:
        conditions = get_weather(config)
        if (conditions):
            print("forecast:", conditions)
            board.write_message(conditions, line=1)
    except:
        print("unable to get weather data")

    def alarm_actions():

        """
        Perform alarm actions.
        Alternates LCD background color and buzzer sound.
        """

        tick = alarm_state["tick"]
        board.change_background("white" if tick else "red")
        if tick:
             board.stop_buzzer() 
        else:
             board.start_buzzer()
        alarm_state["tick"] = not tick
    
    alarm_interval = scheduler.add_job(alarm_actions, "interval", seconds=ms(250))

    def notify(duration):

        """
        Publish data to MQTT server and data store.
        """

        print("Alarm duration (ms):", duration)
        payload = { "value": duration }
        publish_message(config, payload)
        store_message(config, payload)

    def stop_alarm():

        """
        Stop the alarm timer and reset hardare.
        """

        global alarm_time

        total_ms = (utcnow() - alarm_time).total_seconds() * 1000
        notify(total_ms)

        alarm_interval.remove()
        alarm_time = alarm_time.replace(days=1)
        board.change_background("white")
        board.stop_buzzer()

    emitter.once("button-pressed", stop_alarm)

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

    global alarm_time
    global alarm_duration
    print("query:", request.query)
    if { "hour", "minute", "second" } <= set(request.query):
        duration = {
            "hour": int(request.query.get("hour")) or 0,
            "minute": int(request.query.get("minute")) or 0,
            "second": int(request.query.get("second")) or 0
        }
        alarm_time = utcnow().replace(hours=duration["hour"], minutes=duration["minute"], seconds=duration["second"])
        alarm_duration = duration
        print("alarm set to go off at", alarm_time.format("h:mm:ss A"))
    return static_file('index.html', root = "")

@route('/alarm.json')
def serve_json():

    """
    Serve 'alarm.json' with duration to alarm trigger.
    """

    payload = { 
        "hour": alarm_duration["hour"], 
        "minute": alarm_duration["minute"], 
        "second": alarm_duration["second"]
    }
    return payload        

def main():

    """
    Start main function.
    """

    board.stop_buzzer()
    start_clock()
    run(host = "0.0.0.0", port = 5000)

if __name__ == "__main__":
    main()