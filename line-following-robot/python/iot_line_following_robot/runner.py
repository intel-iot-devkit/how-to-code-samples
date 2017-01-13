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
from .log import log
from .scheduler import SCHEDULER, ms

class Runner(object):

    def __init__(self):

        self.project_name = "Line Following Robot"

        board_name = HARDWARE_CONFIG.kit
        board_module = "{0}.hardware.{1}".format(__package__, board_name)
        board_class_name = "{0}Board".format(board_name.capitalize())
        self.board = getattr(import_module(board_module), board_class_name)()

        self.find_direction = "left"

        self.loop_job = SCHEDULER.add_job(
            self.loop,
            "interval",
            seconds=ms(100),
            coalesce=True,
            max_instances=1)

    def loop(self):

        if self.board.detect_line():
            self.move()
        else:
            self.find_line()

    def find_line(self):

        print("Finding line.")

        if self.find_direction == "left":
            self.board.move_left(512)
            self.find_direction = "right"
        else:
            self.board.move_right(512)
            self.find_direction = "left"

    def move(self):

        print("Moving forward.")

        log("moving")
        self.board.move_right(1024)
        self.board.move_left(1024)
