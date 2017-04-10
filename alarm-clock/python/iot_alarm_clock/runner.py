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
from importlib import import_module
from arrow import utcnow
from bottle import Bottle, static_file, request
from twisted.internet import reactor
from pkg_resources import resource_filename
from .config import HARDWARE_CONFIG, WEATHER_CONFIG
from .scheduler import SCHEDULER, ms
from .log import log
from .weather import get_weather
from .hardware.events import ROTARY, BUTTON_RELEASED

class Runner(object):

    def __init__(self):

        self.project_name = "Alarm Clock"

        board_name = HARDWARE_CONFIG.kit
        board_module = "{0}.hardware.{1}".format(__package__, board_name)
        board_class_name = "{0}Board".format(board_name.capitalize())
        self.board = getattr(import_module(board_module), board_class_name)()

        self.board.add_event_handler(ROTARY, self.adjust_brightness)
        self.board.add_event_handler(BUTTON_RELEASED, self.alarm_disarm)

        # setup HTTP server
        self.server = Bottle()
        self.server.route("/", callback=self.serve_index)
        self.server.route("/styles.css", callback=self.serve_css)
        self.server.route("/alarm.json", callback=self.serve_alarm)

        # alarm state
        self.current_time = utcnow().floor("second")
        self.alarm_time = utcnow().floor("second")
        self.alarm_set = False
        self.alarm_disarm_durration = None
        self.alarm_input = {
            "hour": 0,
            "minute": 0,
            "second": 0
        }

        self.alarm_job = None

        self.reset()

    def start(self):

        """
        Start runner.
        """

        SCHEDULER.add_job(self.start_clock, "interval", coalesce=True, seconds=ms(1000))

        reactor.callInThread(lambda: self.server.run(
            host="0.0.0.0",
            port=3000
        ))

    # alarm methods

    def start_clock(self):

        """
        Start clock timer.
        Checks every 50ms to update time and to start the alarm.
        """

        def after(t, c):
            return t.floor("second") >= c.floor("second")

        def same(t, c):
            return t.floor("second") == c.floor("second")

        time = utcnow()
        if after(time, self.current_time):
            self.board.write_message(str(time.format("h:mm:ss A")))
            if self.alarm_set and after(time, self.alarm_time):
                self.alarm_on()

        self.current_time = time

    def alarm_on(self):

        if not self.alarm_job:

            print("Starting alarm.")

            alarm_state = {
                "tick": False
            }

            def alarm_actions():

                tick = alarm_state["tick"]
                if tick:
                    self.board.change_background("red")
                    self.board.start_buzzer()
                else:
                    self.board.change_background("white")
                    self.board.stop_buzzer()
                alarm_state["tick"] = not tick

            SCHEDULER.add_job(self.display_weather)

            self.alarm_job = SCHEDULER.add_job(
                alarm_actions,
                "interval",
                seconds=ms(500),
                coalesce=True,
                max_instances=1
            )

    def alarm_off(self):

        if self.alarm_job:

            print("Stoping alarm.")

            self.alarm_job.remove()
            self.alarm_job = None
            self.reset()

    def alarm_disarm(self):

        if self.alarm_set:

            print("Alarm button pressed.")
            self.alarm_off()
            alarm_disarm_duration = (utcnow() - self.alarm_time)
            print("Alarm runtime: {0}.".format(alarm_disarm_duration))
            log(ms(alarm_disarm_duration.total_seconds()))
            self.alarm_set = False

    def display_weather(self):

        if WEATHER_CONFIG:

            print("Getting weather data.")

            try:
                conditions = get_weather()
                if conditions:
                    print("Forecast: {0}.".format(conditions))
                    self.board.write_message(conditions, line=1)
                else:
                    print("Unable to get weather data.")
            except:
                print("Unable to get weather data.")

    # hardware methods

    def adjust_brightness(self, value):

        """
        Adjust LCD brightness.
        """

        self.board.change_brightness(value)

    def reset(self):

        """
        Reset board state.
        """

        self.board.change_background("white")
        self.board.stop_buzzer()
        self.board.write_message("")
        self.board.write_message("", line=1)

    # HTTP handler methods

    def serve_index(self):

        """
        Serve the 'index.html' file.
        """

        if {"hour", "minute", "second"} <= set(request.query):

            print("Process alarm query parameters: {0}.".format(dict(request.query)))

            raw_hour = int(request.query.get("hour"))
            raw_minute = int(request.query.get("minute"))
            raw_second = int(request.query.get("second"))

            alarm_input = {
                "hour":  0 if raw_hour < 0 else 23 if raw_hour > 23 else raw_hour,
                "minute":  0 if raw_minute < 0 else 59 if raw_minute > 59 else raw_minute,
                "second":  0 if raw_second < 0 else 59 if raw_second > 59 else raw_second
            }

            self.alarm_time = utcnow().replace(
                hour=alarm_input["hour"],
                minute=alarm_input["minute"],
                second=alarm_input["second"]
            )

            self.alarm_input = alarm_input
            self.alarm_set = True

            print("Alarm set to go off at {0}.".format(self.alarm_time.format("h:mm:ss A")))

        resource_package = __name__
        resource_path = "index.html"
        package_root = resource_filename(resource_package, "")
        return static_file(resource_path, root=package_root)

    def serve_css(self):

        """
        Serve the 'styles.css' file.
        """

        resource_package = __name__
        resource_path = "styles.css"
        package_root = resource_filename(resource_package, "")
        return static_file(resource_path, root=package_root)

    def serve_alarm(self):

        """
        Serve 'alarm.json' data.
        """

        payload = {
            "hour": self.alarm_input["hour"],
            "minute": self.alarm_input["minute"],
            "second": self.alarm_input["second"]
        }
        return payload
