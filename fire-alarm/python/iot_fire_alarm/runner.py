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
from .config import HARDWARE_CONFIG, APP_CONFIG
from .hardware.events import TEMPRETURE_CHANGED
from .scheduler import SCHEDULER, ms
from .log import log
from .sms import send_sms

class Runner(object):

    def __init__(self):

        self.project_name = "Fire Alarm"

        board_name = HARDWARE_CONFIG.kit
        board_module = "{0}.hardware.{1}".format(__package__, board_name)
        board_class_name = "{0}Board".format(board_name.capitalize())
        self.board = getattr(import_module(board_module), board_class_name)()

        self.board.add_event_handler(TEMPRETURE_CHANGED, self.monitor_temperature)

        self.alarm_job = None
        self.alarm_tick = False
        self.temperature = 0

        self.reset()

    def monitor_temperature(self, temp):

        current_temperature = temp
        if self.temperature != current_temperature:
            print("Current temperature: {0}.".format(current_temperature))
            self.board.write_message("temperature: {0}".format(current_temperature))
            if current_temperature > APP_CONFIG.alarm_threshold:
                self.start_alarm()
            else:
                self.stop_alarm()

    def start_alarm(self):

        if not self.alarm_job:

            print("Starting fire alarm.")

            log("fire-alarm")

            self.alarm_job = SCHEDULER.add_job(
                self.perform_alarm,
                "interval",
                seconds=ms(250),
                coalesce=True,
                max_instances=1
            )

    def stop_alarm(self):

        if self.alarm_job:
            print("Stoping fire alarm.")
            self.alarm_job.remove()
            self.alarm_job = None
            self.reset()


    def perform_alarm(self):

        self.board.write_message("Fire detected!", line=1)
        if self.alarm_tick:
            self.board.change_background("red")
            self.board.start_buzzer()
        else:
            self.board.change_background("white")
            self.board.stop_buzzer()
        self.alarm_tick = not self.alarm_tick

    def reset(self):
        self.board.change_background("white")
        self.board.stop_buzzer()
        self.board.write_message("")
        self.board.write_message("", line=1)
