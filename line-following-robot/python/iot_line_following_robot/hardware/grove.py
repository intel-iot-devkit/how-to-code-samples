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
from upm.pyupm_uln200xa import ULN200XA, ULN200XA_DIR_CW as DIR_CW, ULN200XA_DIR_CCW as DIR_CCW
from upm.pyupm_linefinder import LineFinder
from mraa import addSubplatform, GENERIC_FIRMATA
from ..config import APP_CONFIG, HARDWARE_CONFIG, KNOWN_PLATFORMS
from .board import Board, PinMappings

class GroveBoard(Board):

    """
    Board class for Grove hardware.
    """

    def __init__(self):

        super(GroveBoard, self).__init__()

        # pin mappings
        self.pin_mappings = PinMappings(
            right_step_in1_pin=9,
            right_step_in2_pin=10,
            right_step_in3_pin=11,
            right_step_in4_pin=12,
            left_step_in1_pin=4,
            left_step_in2_pin=5,
            left_step_in3_pin=6,
            left_step_in4_pin=7,
            line_finder_pin=2,
            i2c_bus=6
        )

        if HARDWARE_CONFIG.platform == KNOWN_PLATFORMS.firmata:
            addSubplatform(GENERIC_FIRMATA, "/dev/ttyACM0")
            self.pin_mappings += 512
            self.pin_mappings.i2c_bus = 512

        self.right_step = ULN200XA(
            4096,
            self.pin_mappings.right_step_in1_pin,
            self.pin_mappings.right_step_in2_pin,
            self.pin_mappings.right_step_in3_pin,
            self.pin_mappings.right_step_in4_pin
        )

        self.left_step = ULN200XA(
            4096,
            self.pin_mappings.left_step_in1_pin,
            self.pin_mappings.left_step_in2_pin,
            self.pin_mappings.left_step_in3_pin,
            self.pin_mappings.left_step_in4_pin
        )

        self.line_finder = LineFinder(self.pin_mappings.line_finder_pin)


    def update_hardware_state(self):

        """
        Update hardware state.
        """

        pass

    # hardware functions
    def detect_line(self):

        if APP_CONFIG.white_lines == 0:
            return self.line_finder.whiteDetected()
        else:
            return self.line_finder.blackDetected()

    def move_right(self, steps=1024, step_dir=APP_CONFIG.clockwise, speed=5):

        self.right_step.setSpeed(speed)
        self.right_step.setDirection(step_dir)
        self.right_step.stepperSteps(steps)

    def move_left(self, steps=1024, step_dir=APP_CONFIG.clockwise, speed=5):

        self.left_step.setSpeed(speed)
        self.left_step.setDirection(step_dir)
        self.left_step.stepperSteps(steps)
