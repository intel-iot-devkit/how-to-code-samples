# Copyright (c) 2015 - 2017 Intel Corporation.
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
from datetime import datetime, timedelta
from importlib import import_module
from pkg_resources import resource_filename
from bottle import Bottle, static_file, request
from .hardware.events import TEMP_READING, FLAME_DETECTED
from .config import HARDWARE_CONFIG, APP_CONFIG
from .log import log

class Runner(object):

    def __init__(self):

        self.project_name = "Smart Stove Top"

        board_name = HARDWARE_CONFIG.kit
        board_module = "{0}.hardware.{1}".format(__package__, board_name)
        board_class_name = "{0}Board".format(board_name.capitalize())
        self.board = getattr(import_module(board_module), board_class_name)()

        self.board.add_event_handler(TEMP_READING, self.update_temp)
        self.board.add_event_handler(FLAME_DETECTED, self.update_flame)

        # setup HTTP server
        self.server = Bottle()
        self.server.route("/", callback=self.serve_index)
        self.server.route("/styles.css", callback=self.serve_css)
        self.server.route("/temp.json", callback=self.serve_temp)

        # hardware state
        self.temp_triggered = False
        self.flame_triggered = False

        self.log_triggered = False
        self.last_log = datetime.utcnow()
        self.log_interval = timedelta(seconds=5)

        self.target_temp = APP_CONFIG.target_temp

    def start(self):

        """
        Start runner.
        """

        self.server.run(
            host="0.0.0.0",
            port=3000
        )

    # hardware methods

    def update_temp(self, temp):

        should_trigger = temp >= self.target_temp
        print("temp:", temp)

        if should_trigger and not self.temp_triggered:
            print("Temperature alarm triggered.")
            self.temp_triggered = True
            self.log_triggered = True
            self.board.play_temp()

        if self.log_triggered:
            log_delta = datetime.utcnow() - self.last_log
            if log_delta >= self.log_interval:
                log(temp)
                self.last_log = datetime.utcnow()

        if not should_trigger and self.temp_triggered:
            self.temp_triggered = False

    def update_flame(self, flame_detected):

        should_trigger = flame_detected is True

        if should_trigger and not self.flame_triggered:
            print("Flame alarm triggered.")
            self.flame_triggered = True
            self.board.play_flame()

        if not should_trigger and self.flame_triggered:
            self.flame_triggered = False

    # server methods

    def serve_index(self):

        """
        Serve the 'index.html' file.
        """

        if {"temp"} <= set(request.query):

            print("Process temp query parameter: {0}.".format(dict(request.query)))
            self.target_temp = int(request.query.get("temp"))

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

    def serve_temp(self):

        """
        Serve 'temp.json' data.
        """

        payload = {
            "temp": self.target_temp
        }

        return payload
