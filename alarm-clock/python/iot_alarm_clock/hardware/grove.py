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
from math import floor
from upm.pyupm_i2clcd import Jhd1313m1
from upm.pyupm_button import Button
from upm.pyupm_rotary import Rotary
from mraa import Gpio, DIR_OUT, addSubplatform, GENERIC_FIRMATA
from ..config import HARDWARE_CONFIG, KNOWN_PLATFORMS
from .board import Board, PinMappings
from .events import ROTARY, BUTTON_RELEASED

class GroveBoard(Board):

    """
    Board class for Grove hardware.
    """

    def __init__(self):

        super(GroveBoard, self).__init__()

        # pin mappings
        self.pin_mappings = PinMappings(
            buzzer_pin=6,
            button_pin=5,
            rotary_pin=0,
            i2c_bus=6
        )

        if HARDWARE_CONFIG.platform == KNOWN_PLATFORMS.firmata:
            addSubplatform(GENERIC_FIRMATA, "/dev/ttyACM0")
            self.pin_mappings += 512
            self.pin_mappings.i2c_bus = 512

        self.screen = Jhd1313m1(self.pin_mappings.i2c_bus, 0x3E, 0x62)
        self.buzzer = Gpio(self.pin_mappings.buzzer_pin)
        self.rotary = Rotary(self.pin_mappings.rotary_pin)
        self.button = Button(self.pin_mappings.button_pin)

        self.buzzer.dir(DIR_OUT)

        self.rotary_value = 0
        self.button_state = False


    def update_hardware_state(self):

        """
        Update hardware state.
        """

        current_rotary_value = self.read_rotary()
        if current_rotary_value != self.rotary_value:
            self.trigger_hardware_event(ROTARY, current_rotary_value)
            self.rotary_value = current_rotary_value

        current_button_state = self.button.value()
        if current_button_state != self.button_state:
            if not current_button_state:
                self.trigger_hardware_event(BUTTON_RELEASED)
            self.button_state = current_button_state

    # hardware functions
    def start_buzzer(self):

        """
        Start the hardware buzzer.
        """

        self.buzzer.write(1)

    def stop_buzzer(self):

        """
        Stop the hardware buzzer.
        """

        self.buzzer.write(0)

    def read_rotary(self):

        """
        Read rotary value.
        """

        return self.rotary.abs_value()

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
            "purple": lambda: self.screen.setColor(255, 0, 255),
            "blue": lambda: self.screen.setColor(0, 0, 255),
            "green": lambda: self.screen.setColor(0, 255, 0),
            "yellow": lambda: self.screen.setColor(255, 255, 0),
            "white": lambda: self.screen.setColor(255, 255, 255)
        }
        colors.get(color, colors["white"])()

    def change_brightness(self, value):

        """
        Change LCD screen brightness.
        """

        start = 0
        end = 1020
        value = int(floor((value - start) / end * 255))
        value = 0 if value < 0 else 255 if value > 255 else value
        self.screen.setColor(value, value, value)
