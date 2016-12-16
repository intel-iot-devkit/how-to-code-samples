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
from .hardware.grove import STEP_DIRECTION
from .hardware.events import JOYSTICK_READING

class Runner(object):

    def __init__(self):

        self.project_name = "Robot Arm"

        board_name = HARDWARE_CONFIG.kit
        board_module = "{0}.hardware.{1}".format(__package__, board_name)
        board_class_name = "{0}Board".format(board_name.capitalize())
        self.board = getattr(import_module(board_module), board_class_name)()

        self.board.add_event_handler(JOYSTICK_READING, self.handle_joystick)

    def scale(self, data):

        val = (data - -0.5) / -0.4
        val = 1 if val > 1 else 0 if val < 0 else val
        return round(val * 2 - 1)

    def handle_joystick(self, data):

        x_input = self.scale(data.x)
        y_input = self.scale(data.y)

        print("x input:", x_input)
        print("y input:", y_input)

        if x_input == 1:
            self.board.move(1, STEP_DIRECTION.cw)
        if x_input == -1:
            self.board.move(1, STEP_DIRECTION.ccw)

        if y_input == 1:
            self.board.move(2, STEP_DIRECTION.cw)
        if y_input == -1:
            self.board.move(2, STEP_DIRECTION.ccw)
