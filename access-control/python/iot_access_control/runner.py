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
from collections import namedtuple
from datetime import datetime, timedelta
from pkg_resources import Requirement, resource_filename
from bottle import Bottle, static_file, request, HTTPResponse
from twisted.internet import reactor
from .config import HARDWARE_CONFIG, APP_CONFIG
from .hardware.events import MOTION_DETECTED
from .scheduler import SCHEDULER, ms
from .log import log

class Runner(object):

    """
    Runner class implements the alarm logic.

    The alarm goes through the following states:
        - looking for motion
        - motion detected, waiting for code
            + code validated -> disarm
            + grace timeout -> trigger Alarm
                - code validated -> disarm
    """

    AlarmState = namedtuple("AlarmState", "scanning motion_detected alarm_triggered")
    ALARM_STATE = AlarmState(
        scanning=1,
        motion_detected=2,
        alarm_triggered=3
    )

    def __init__(self):

        self.project_name = "Access Control"

        self.server = Bottle()
        self.server.route("/", callback=self.serve_index)
        self.server.route("/alarm", callback=self.serve_validation)

        board_name = HARDWARE_CONFIG.kit
        board_module = "{0}.hardware.{1}".format(__package__, board_name)
        board_class_name = "{0}Board".format(board_name.capitalize())
        self.board = getattr(import_module(board_module), board_class_name)()

        self.code = APP_CONFIG.code
        self.alarm_state = Runner.ALARM_STATE.scanning
        self.previous_time = datetime.utcnow()
        self.validated = False
        self.grace_timeout = timedelta(seconds=30)
        self.alarm_triggered = False
        self.motion_detected = False

        def update_motion_state():

            """
            Update motion detected state.
            """

            if not self.motion_detected:
                log("motion-detected")
                self.motion_detected = True
                self.previous_time = datetime.utcnow()

        self.board.add_event_handler(MOTION_DETECTED, update_motion_state)

        self.board.change_background("white")
        self.board.write_message("scanning")

        self.alarm_monitor_job = SCHEDULER.add_job(
            self.monitor_alarm,
            "interval",
            seconds=ms(1000),
            coalesce=True,
            max_instances=1
        )

    # server methods

    def start(self):

        """
        Start runner.
        """

        reactor.callInThread(lambda: self.server.run(
            host="0.0.0.0",
            port=3000
        ))

    def serve_index(self):

        """
        Serve the 'index.html' file.
        """

        resource_package = __name__
        resource_path = "index.html"
        package_root = resource_filename(resource_package, "")
        return static_file(resource_path, root=package_root)

    def serve_validation(self):

        """
        Handle alarm code validation.
        """

        if {"code"} <= set(request.query):
            code = request.query.get("code")

            if self.validate_code(code):
                return HTTPResponse(status=200)
            else:
                return HTTPResponse(status=403)

        return HTTPResponse(status=400)

    # alarm methods

    def monitor_alarm(self):

        """
        Monitor alarm state.
        """

        if self.motion_detected:

            # check if alarm should disarm
            if self.validated:
                self.stop_alarm()
                return

            # handle alarm state
            current_time = datetime.utcnow()
            grace_expired = (current_time - self.previous_time) > self.grace_timeout

            # check if alarm should trigger
            if not grace_expired:
                self.require_validation()
            else:
                self.start_alarm()

    def require_validation(self):

        """
        Require validation code.
        """

        if self.alarm_state != Runner.ALARM_STATE.motion_detected:
            self.alarm_state = Runner.ALARM_STATE.motion_detected
            self.board.change_background("blue")
            self.board.write_message("motion detected")

    def start_alarm(self):

        """
        Start alarm.
        """

        if self.alarm_state != Runner.ALARM_STATE.alarm_triggered:
            log("alarm-starting")
            self.alarm_state = Runner.ALARM_STATE.alarm_triggered
            self.alarm_triggered = True
            self.board.change_background("red")
            self.board.write_message("ALARM")

    def stop_alarm(self):

        """
        Stop alarm.
        """

        if self.alarm_state != Runner.ALARM_STATE.scanning:
            log("alarm-stopping")
            self.alarm_state = Runner.ALARM_STATE.scanning
            self.alarm_triggered = False
            self.validated = False
            self.motion_detected = False
            self.board.change_background("white")
            self.board.write_message("scanning")

    def validate_code(self, code):

        """
        Validate alarm access code.
        """

        validation_result = True if self.code == code else False
        if validation_result:
            log("validated-entry")
        self.validated = validation_result
        return validation_result
