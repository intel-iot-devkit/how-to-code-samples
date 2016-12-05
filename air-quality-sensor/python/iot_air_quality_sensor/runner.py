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
from .hardware.events import AIR_QUALITY
from .log import log

class Runner(object):

    def __init__(self):

        self.project_name = "Air Quality"

        board_name = HARDWARE_CONFIG.kit
        board_module = "{0}.hardware.{1}".format(__package__, board_name)
        board_class_name = "{0}Board".format(board_name.capitalize())
        self.board = getattr(import_module(board_module), board_class_name)()

        self.board.add_event_handler(AIR_QUALITY, self.monitor_air)

        self.triggered = False

    def monitor_air(self, air_quality):

        print("Air quality: {0}.".format(air_quality))

        if air_quality > APP_CONFIG.threshold and not self.triggered:

            print("Air quality triggered.")

            self.triggered = True
            self.board.play_chime()
            log("Air quality alert")

        elif air_quality <= APP_CONFIG.threshold and self.triggered:

            print("Air quality disarmed.")
            self.triggered = False
