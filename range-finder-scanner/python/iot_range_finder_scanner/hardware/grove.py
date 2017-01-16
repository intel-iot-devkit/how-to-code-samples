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
from upm.pyupm_uln200xa import ULN200XA
from upm.pyupm_rfr359f import RFR359F
from mraa import addSubplatform, GENERIC_FIRMATA
from ..config import HARDWARE_CONFIG, KNOWN_PLATFORMS
from .board import Board, PinMappings

class GroveBoard(Board):

    """
    Board class for Grove hardware.
    """

    def __init__(self):

        super(GroveBoard, self).__init__()

        # pin mappings
        self.pin_mappings = PinMappings(
            step_in1_pin=9,
            step_in2_pin=10,
            step_in3_pin=11,
            step_in4_pin=12,
            distance_pin=2
        )

        if HARDWARE_CONFIG.platform == KNOWN_PLATFORMS.firmata:
            addSubplatform(GENERIC_FIRMATA, "/dev/ttyACM0")
            self.pin_mappings += 512

        self.distance = RFR359F(self.pin_mappings.distance_pin)

        self.step = ULN200XA(
            4096,
            self.pin_mappings.step_in1_pin,
            self.pin_mappings.step_in2_pin,
            self.pin_mappings.step_in3_pin,
            self.pin_mappings.step_in4_pin
        )

    def update_hardware_state(self):

        """
        Update hardware state.
        """

        pass

    # hardware functions
    def move(self):

        self.step.setSpeed(5)
        self.step.setDirection(0)
        self.step.stepperSteps(11)

    def detect_object(self):

        return self.distance.objectDetected()
