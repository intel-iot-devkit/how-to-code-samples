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
from upm.pyupm_i2clcd import SAINSMARTKS
from upm.pyupm_loudness import Loudness
from mraa import addSubplatform, GENERIC_FIRMATA
from ..config import HARDWARE_CONFIG, KNOWN_PLATFORMS
from .board import Board, PinMappings
from .events import SOUND_MEASUREMENT

class DfrobotBoard(Board):

    """
    Board class for drobot hardware.
    """

    def __init__(self):

        super(DfrobotBoard, self).__init__()

        # pin mappings
        self.pin_mappings = PinMappings(
            mic_pin=1,
            screen_register_select_pin=8,
            screen_enable_pin=9,
            screen_data_0_pin=4,
            screen_data_1_pin=5,
            screen_data_2_pin=6,
            screen_data_3_pin=7,
            screen_analog_input_pin=0
        )

        if HARDWARE_CONFIG.platform == KNOWN_PLATFORMS.firmata:
            addSubplatform(GENERIC_FIRMATA, "/dev/ttyACM0")
            self.pin_mappings += 512

        self.screen = SAINSMARTKS(
            self.pin_mappings.screen_register_select_pin,
            self.pin_mappings.screen_enable_pin,
            self.pin_mappings.screen_data_0_pin,
            self.pin_mappings.screen_data_1_pin,
            self.pin_mappings.screen_data_2_pin,
            self.pin_mappings.screen_data_3_pin,
            self.pin_mappings.screen_analog_input_pin
        )

        self.mic = Loudness(self.pin_mappings.mic_pin)

    def update_hardware_state(self):

        """
        Update hardware state.
        """

        self.trigger_hardware_event(SOUND_MEASUREMENT, self.measure_volume())

    # hardware functions
    def measure_volume(self):

        """
        Measure the average volume
        """

        noise = self.mic.loudness()
        in_min = 0
        in_max = 5
        out_min = 0
        out_max = 100
        average = (noise - in_min) * (out_max - out_min) / (in_max - in_min) + out_min
        return round(average, 2)

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
        No effect on the dfrobot.
        """

        pass
