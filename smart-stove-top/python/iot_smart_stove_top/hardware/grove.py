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
from upm.pyupm_speaker import Speaker
from upm.pyupm_otp538u import OTP538U
from upm.pyupm_yg1006 import YG1006
from mraa import addSubplatform, GENERIC_FIRMATA
from ..config import HARDWARE_CONFIG, KNOWN_PLATFORMS
from .board import Board, PinMappings
from .events import TEMP_READING, FLAME_DETECTED

class GroveBoard(Board):

    """
    Board class for Grove hardware.
    """

    def __init__(self):

        super(GroveBoard, self).__init__()

        # pin mappings
        self.pin_mappings = PinMappings(
            speaker_pin=5,
            flame_pin=4,
            temp_obj_pin=1,
            temp_ambient_pin=0,
            i2c_bus=6,
            reference_voltage=5.0
        )

        if HARDWARE_CONFIG.platform == KNOWN_PLATFORMS.firmata:
            addSubplatform(GENERIC_FIRMATA, "/dev/ttyACM0")
            self.pin_mappings += 512
            self.pin_mappings.i2c_bus = 512
            self.pin_mappings.reference_voltage = 3.3

        self.speaker = Speaker(self.pin_mappings.speaker_pin)
        self.flame = YG1006(self.pin_mappings.flame_pin)
        self.temp = OTP538U(
            self.pin_mappings.temp_ambient_pin,
            self.pin_mappings.temp_obj_pin,
            self.pin_mappings.reference_voltage
        )


    def update_hardware_state(self):

        """
        Update hardware state.
        """

        obj_temp = self.read_obj_temp()
        self.trigger_hardware_event(TEMP_READING, obj_temp)

        flame_detected = self.detect_flame()
        self.trigger_hardware_event(FLAME_DETECTED, flame_detected)

    # hardware functions
    def read_obj_temp(self):

        return self.temp.objectTemperature()

    def detect_flame(self):

        return self.flame.flameDetected()

    def play_temp(self):

        self.speaker.playSound("a", True, "low")
        self.speaker.playSound("c", True, "low")
        self.speaker.playSound("d", True, "low")
        self.speaker.playSound("b", False, "low")

    def play_flame(self):

        for _ in range(0, 10):
            self.speaker.playSound("a", True, "high")
            self.speaker.playSound("c", True, "high")
            self.speaker.playSound("g", True, "med")
