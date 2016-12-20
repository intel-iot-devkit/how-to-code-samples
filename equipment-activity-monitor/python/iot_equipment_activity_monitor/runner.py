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
from .hardware.events import NOISE_SAMPLE, VIBRATION_SAMPLE
from .scheduler import SCHEDULER, ms
from .log import log

class Runner(object):

    def __init__(self):

        self.project_name = "Equipment Activity Monitor"

        board_name = HARDWARE_CONFIG.kit
        board_module = "{0}.hardware.{1}".format(__package__, board_name)
        board_class_name = "{0}Board".format(board_name.capitalize())
        self.board = getattr(import_module(board_module), board_class_name)()

        self.sound_measurement = 0
        self.vibration_measurement = 0

        self.board.add_event_handler(NOISE_SAMPLE, self.monitor_sound)
        self.board.add_event_handler(VIBRATION_SAMPLE, self.monitor_vibration)

        self.display_clear()

        self.triggered = False

        self.monitor_job = SCHEDULER.add_job(
            self.monitor_activity,
            "interval",
            seconds=ms(1000),
            coalesce=True,
            max_instances=1)

    def monitor_activity(self):

        vibration_active = self.vibration_measurement > APP_CONFIG.vibration_threshold
        sound_active = self.sound_measurement > APP_CONFIG.noise_threshold

        if vibration_active and sound_active:
            if not self.triggered:
                print("Equipment use detected.")
                log("equipment-start")
                self.display_active()
                self.triggered = True
        elif self.triggered:
            log("equipment-stop")
            self.display_clear()
            self.triggered = False

    def monitor_sound(self, sound):

        print("Sound measurement: {0}.".format(sound))
        self.sound_measurement = sound

    def monitor_vibration(self, vibration):

        print("Vibration measurement: {0}.".format(vibration))
        self.vibration_measurement = vibration

    def display_active(self):

        self.board.write_message("EQUIPMENT IN USE")
        self.board.change_background("white")

    def display_clear(self):

        self.board.write_message("")
        self.board.change_background("clear")
