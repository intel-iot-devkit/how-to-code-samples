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
from datetime import datetime, timedelta
from upm.pyupm_moisture import Moisture
from mraa import Gpio, DIR_OUT, addSubplatform, GENERIC_FIRMATA
from ..config import HARDWARE_CONFIG, KNOWN_PLATFORMS
from ..scheduler import SCHEDULER, ms
from .board import Board, PinMappings
from .events import MOISTURE_SAMPLE

class DfrobotBoard(Board):

    """
    Board class for drobot hardware.
    """

    def __init__(self):

        super(DfrobotBoard, self).__init__()

        # pin mappings
        self.pin_mappings = PinMappings(
            moisture_pin=0,
            speaker_pin=16
        )

        if HARDWARE_CONFIG.platform == KNOWN_PLATFORMS.firmata:
            addSubplatform(GENERIC_FIRMATA, "/dev/ttyACM0")
            self.pin_mappings += 512

        self.moisture = Moisture(self.pin_mappings.moisture_pin)
        self.speaker = Gpio(self.pin_mappings.speaker_pin)

        self.speaker.dir(DIR_OUT)

    def update_hardware_state(self):

        """
        Update hardware state.
        """

        moisture_reading = self.read_moisture()
        self.trigger_hardware_event(MOISTURE_SAMPLE, moisture_reading)

    # hardware functions
    def read_moisture(self):

        return self.moisture.value()

    def play_sound(self):

        self.speaker.write(1)
        SCHEDULER.add_job(
            lambda: self.speaker.write(0),
            "date",
            run_date=datetime.utcnow() + timedelta(seconds=1)
        )
