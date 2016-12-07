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
from .config import HARDWARE_CONFIG
from .hardware.events import SOUND_MEASUREMENT
from .log import log
from .scheduler import SCHEDULER, ms

class Runner(object):

    def __init__(self):

        self.project_name = "Sound Detector"

        board_name = HARDWARE_CONFIG.kit
        board_module = "{0}.hardware.{1}".format(__package__, board_name)
        board_class_name = "{0}Board".format(board_name.capitalize())
        self.board = getattr(import_module(board_module), board_class_name)()

        self.board.add_event_handler(SOUND_MEASUREMENT, self.display_sound_measurement)
        SCHEDULER.add_job(
            self.log_sound_measurement,
            "interval",
            seconds=ms(5000),
            coalesce=True)

    def display_sound_measurement(self, volume):

        """
        Display average volume measurement.
        """

        self.volume = volume
        print("volume:", volume)

        self.board.write_message("avg volume: " + str(volume))

        if volume < 1:
            self.board.change_background("white")
        elif volume < 2:
            self.board.change_background("yellow")
        elif volume < 3:
            self.board.change_background("green")
        elif volume < 4:
            self.board.change_background("blue")
        elif volume < 5:
            self.board.change_background("purple")
        else:
            self.board.change_background("red")

    def log_sound_measurement(self):

        """
        Log average volume measurement.
        """

        log("avg volume: " + str(self.volume))
