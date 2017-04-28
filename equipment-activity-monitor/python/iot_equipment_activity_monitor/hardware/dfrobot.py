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
from upm.pyupm_lcdks import LCDKS
from upm.pyupm_button import Button
from upm.pyupm_mic import Microphone, thresholdContext, uint16Array
from mraa import addSubplatform, GENERIC_FIRMATA
from ..config import HARDWARE_CONFIG, KNOWN_PLATFORMS
from .board import Board, PinMappings
from .events import VIBRATION_SAMPLE, NOISE_SAMPLE

class DfrobotBoard(Board):

    """
    Board class for drobot hardware.
    """

    def __init__(self):

        super(DfrobotBoard, self).__init__()

        # pin mappings
        self.pin_mappings = PinMappings(
            sound_pin=2,
            vibration_pin=15,
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

        self.screen = LCDKS(
            self.pin_mappings.screen_register_select_pin,
            self.pin_mappings.screen_enable_pin,
            self.pin_mappings.screen_data_0_pin,
            self.pin_mappings.screen_data_1_pin,
            self.pin_mappings.screen_data_2_pin,
            self.pin_mappings.screen_data_3_pin,
            self.pin_mappings.screen_analog_input_pin
        )

        self.sound = Microphone(self.pin_mappings.sound_pin)
        self.sound_ctx = thresholdContext()
        self.sound_ctx.averageReading = 0
        self.sound_ctx.runningAverage = 0
        self.sound_ctx.averagedOver = 2

        self.vibration = Button(self.pin_mappings.vibration_pin)

        self.sample_time = 2
        self.sample_number = 128

    def update_hardware_state(self):

        """
        Update hardware state.
        """

        vibration_sample = self.measure_vibration()
        self.trigger_hardware_event(VIBRATION_SAMPLE, vibration_sample)

        noise_sample = self.measure_sound()
        self.trigger_hardware_event(NOISE_SAMPLE, noise_sample)

    # hardware functions
    def measure_vibration(self):

        """
        Measure average vibration.
        """

        vibration = self.vibration.value()
        return 200 if vibration else 0

    def measure_sound(self):

        """
        Measure average volume.
        """

        samples = uint16Array(128)
        length = self.sound.getSampledWindow(self.sample_time, self.sample_number, samples)

        if not length:
            return 0

        noise = self.sound.findThreshold(self.sound_ctx, 30, samples, length)
        average = noise / 100
        return average

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
