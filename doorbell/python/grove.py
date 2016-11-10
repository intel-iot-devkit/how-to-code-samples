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

from pyupm_ttp223 import TTP223
from pyupm_buzzer import Buzzer
from pyupm_i2clcd import Jhd1313m1

from mraa import addSubplatform, GENERIC_FIRMATA

from constants.hardware import TOUCH_DETECTED

from scheduler import scheduler, ms
from board import Board

class GroveBoard(Board):

    """
    Board class for Grove hardware.
    """

    def __init__(self, config):

        super(GroveBoard, self).__init__()

        # pin mappings
        self.touch_pin = 4
        self.buzzer_pin = 5
        self.i2c_bus = 6

        if "platform" in config and config["platform"] == "firmata":
            addSubplatform("firmata", "/dev/ttyACM0")
            self.touch_pin += 512
            self.buzzer_pin += 512
            self.i2c_bus += 512

        self.touch = TTP223(self.touch_pin)
        self.buzzer = Buzzer(self.buzzer_pin)
        self.screen = Jhd1313m1(self.i2c_bus, 0x3E, 0x62)

    def update_hardware_state(self):

        """
        Update hardware state.
        """

        pass

    # hardware functions
    def write_message(self, message, line=0):

        """
        Write message to LCD screen.
        """

        message = message.ljust(16)
        self.screen.setCursor(line, 0)
        self.screen.write(message)

    def change_background(self, color):

        """
        Change LCD screen background color.
        """

        colors = {
            "red": lambda: self.screen.setColor(255, 0, 0),
            "green": lambda: self.screen.setColor(0, 255, 0),
            "blue": lambda: self.screen.setColor(0, 0, 255),
            "white": lambda: self.screen.setColor(255, 255, 255)
        }
        colors.get(color, colors["white"])()
