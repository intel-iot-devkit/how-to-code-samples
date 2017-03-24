# Copyright (c) 2015 - 2017 Intel Corporation.
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
from datetime import datetime, timedelta
from mraa import Gpio, DIR_OUT, Aio, addSubplatform, GENERIC_FIRMATA
from ..config import HARDWARE_CONFIG, KNOWN_PLATFORMS
from ..scheduler import SCHEDULER
from .board import Board, PinMappings
from .events import TEMP_READING, FLAME_DETECTED

class DfrobotBoard(Board):

    """
    Board class for drobot hardware.
    """

    def __init__(self):

        super(DfrobotBoard, self).__init__()

        # pin mappings
        self.pin_mappings = PinMappings(
            temp_pin=3,
            flame_pin=2,
            speaker_pin=15,
            reference_voltage=1.0
        )

        if HARDWARE_CONFIG.platform == KNOWN_PLATFORMS.firmata:
            addSubplatform(GENERIC_FIRMATA, "/dev/ttyACM0")
            self.pin_mappings += 512
            self.pin_mappings.reference_voltage = 0.33

        self.temp = Aio(self.pin_mappings.temp_pin)
        self.flame = Aio(self.pin_mappings.flame_pin)
        self.speaker = Gpio(self.pin_mappings.speaker_pin)

        self.speaker.dir(DIR_OUT)

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

        return ((500 * self.temp.read()) / 1024) * self.pin_mappings.reference_voltage

    def detect_flame(self):

        return (self.flame.read() * self.pin_mappings.reference_voltage >= 300)

    def play_beep(self):

        self.speaker.write(1)
        SCHEDULER.add_job(
            lambda: self.speaker.write(0),
            "date",
            run_date=datetime.utcnow() + timedelta(seconds=1)
        )

    def play_temp(self):

        self.play_beep()

    def play_flame(self):

        self.play_beep()
