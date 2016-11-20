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

from datetime import datetime, timedelta

from constants.config import CODE
from constants.hardware import MOTION_DETECTED

from scheduler import scheduler, ms
from log import log

class Alarm(object):

    """
    Alarm class implements Alarm logic.

    The alarm goes through the following states:
        - looking for motion
        - motion detected, waiting for code
            + code validated -> disarm
            + grace timeout -> trigger Alarm
                - code validated -> disarm
    """

    class State(object):
        scanning = 0
        motion_detected = 1
        alarm_triggered = 2

    def __init__(self, config, board):

        self.config = config
        self.board = board

        self.code = config.get(CODE, "1234")

        self.alarm_state = Alarm.State.scanning

        self.previous_time = datetime.utcnow()

        self.validated = False
        self.grace_timeout = timedelta(seconds=30)

        self.alarm_triggered = False

        def update_motion_state():
            if not self.motion_detected:
                log(self.config, "motion-detected")
                self.motion_detected = True
                self.previous_time = datetime.utcnow()

        self.motion_detected = False
        self.board.add_event_handler(MOTION_DETECTED, update_motion_state)
        
        self.board.change_background("white")
        self.board.write_message("scanning")

        self.alarm_monitor_job = scheduler.add_job(self.monitor_alarm, "interval", seconds=ms(1000), coalesce=True, max_instances=1)

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

        if self.alarm_state != Alarm.State.motion_detected:
            self.alarm_state = Alarm.State.motion_detected
            self.board.change_background("blue")
            self.board.write_message("motion detected")

    def start_alarm(self):

        """
        Start alarm.
        """

        if self.alarm_state != Alarm.State.alarm_triggered:
            log(self.config, "alarm-starting")
            self.alarm_state = Alarm.State.alarm_triggered
            self.alarm_triggered = True
            self.board.change_background("red")
            self.board.write_message("ALARM")

    def stop_alarm(self):

        """
        Stop alarm.
        """
        
        if self.alarm_state != Alarm.State.scanning:
            log(self.config, "alarm-stopping")
            self.alarm_state = Alarm.State.scanning
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
            log(self.config, "validated-entry")
        self.validated = validation_result
        return validation_result
