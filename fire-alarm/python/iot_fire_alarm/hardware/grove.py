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
from upm.pyupm_i2clcd import Jhd1313m1
from upm.pyupm_temperature import Temperature
from mraa import Gpio, DIR_OUT, addSubplatform, GENERIC_FIRMATA
from ..config import HARDWARE_CONFIG, KNOWN_PLATFORMS
from .board import Board, PinMappings
from .events import TEMPRETURE_CHANGED

class GroveBoard(Board):

    """
    Board class for Grove hardware.
    """

    def __init__(self):

        super(GroveBoard, self).__init__()

        # pin mappings
        self.pin_mappings = PinMappings(
            temperature_pin=0,
            buzzer_pin=5,
            i2c_bus=6
        )
        self.voltage_adjust = 1.0

        if HARDWARE_CONFIG.platform == KNOWN_PLATFORMS.firmata:
            addSubplatform(GENERIC_FIRMATA, "/dev/ttyACM0")
            self.pin_mappings += 512
            self.pin_mappings.i2c_bus = 512
            self.voltage_adjust = 0.66

        self.screen = Jhd1313m1(self.pin_mappings.i2c_bus, 0x3E, 0x62)
        self.temperature = Temperature(self.pin_mappings.temperature_pin)
        self.buzzer = Gpio(self.pin_mappings.buzzer_pin)

        self.buzzer.dir(DIR_OUT)

    def update_hardware_state(self):

        """
        Update hardware state.
        """

        current_temp = self.read_temperature()
        self.trigger_hardware_event(TEMPRETURE_CHANGED, current_temp)

    # hardware functions
    def read_temperature(self):

        """
        Read temperature value in Celcius.
        """

        return self.temperature.value() * self.voltage_adjust

    def start_buzzer(self):

        """
        Start buzzer.
        """

        self.buzzer.write(1)

    def stop_buzzer(self):

        """
        Stop buzzer.
        """

        self.buzzer.write(0)

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
