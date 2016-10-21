from __future__ import print_function, division

# imports
from simplejson import load as load_json
from importlib import import_module
from datetime import datetime, timedelta
from bottle import route, request, run, static_file
from arrow import utcnow

from events import scheduler, emitter, ms

# load config.json data
with open("config.json") as data:
    config = load_json(data)

if "kit" in config:
    print("loading board config \"", config["kit"], "\" from config.json", sep = "")
    board = import_module(config["kit"])
else:
    print("loading default board config")
    board = import_module("grove")

# clock functions
current_time = utcnow().replace(seconds=-1)
alarm_time = utcnow().replace(days=-1)

def after(t, c):
    return t.floor("second") > c.floor("second")

def same(t, c):
    return t.floor("second") == c.floor("second")

def start_clock():
    global current_time
    time = utcnow()
    if after(time, current_time):
        board.write_message(str(time.format("h:mm:ss A")))
        if same(time, alarm_time):
            start_alarm()

    current_time = time

scheduler.add_job(start_clock, "interval", seconds=ms(50))

def start_alarm():
    global alarm_time

    alarm_state = {
        "tick": False
    }

    board.start_buzzer()
    board.set_screen_background("red")

    def alarm_actions():
        tick = alarm_state["tick"]
        board.set_screen_background("white" if tick else "red")
        if tick:
             board.stop_buzzer() 
        else:
             board.start_buzzer()
        alarm_state["tick"] = not tick
    
    print("schedule alarm_actions")
    alarm_interval = scheduler.add_job(alarm_actions, "interval", seconds=ms(250))

    def stop_alarm():
        global alarm_time
        alarm_interval.remove()
        alarm_time = alarm_time.replace(days=1)
        board.set_screen_background("white")
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
    payload = { 
        "hour": alarm_duration["hour"], 
        "minute": alarm_duration["minute"], 
        "second": alarm_duration["second"]
    }
    return payload        

def main():
    board.init_hardware(config)
    board.stop_buzzer()
    start_clock()
    run(host = "0.0.0.0", port = 5000)

if __name__ == "__main__":
    main()