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
from upm.pyupm_gas import MQ2
from mraa import addSubplatform, GENERIC_FIRMATA, Gpio, DIR_OUT
from ..config import HARDWARE_CONFIG, KNOWN_PLATFORMS
from .board import Board, PinMappings
from .events import AIR_QUALITY
from ..scheduler import SCHEDULER

class DfrobotBoard(Board):

    """
    Board class for drobot hardware.
    """

    def __init__(self):

        super(DfrobotBoard, self).__init__()

        # pin mappings
        self.pin_mappings = PinMappings(
            gas_pin=2,
            speaker_pin=15
        )

        if HARDWARE_CONFIG.platform == KNOWN_PLATFORMS.firmata:
            addSubplatform(GENERIC_FIRMATA, "/dev/ttyACM0")
            self.pin_mappings += 512

        self.air = MQ2(self.pin_mappings.gas_pin)

        self.speaker = Gpio(self.pin_mappings.speaker_pin)
        self.speaker.dir(DIR_OUT)

    def update_hardware_state(self):

        """
        Update hardware state.
        """

        current_air_quality = self.sample_air()
        self.trigger_hardware_event(AIR_QUALITY, current_air_quality)

    # hardware functions
    def play_chime(self):

        """
        Play chime.
        """

        self.speaker.write(1)
        SCHEDULER.add_job(
            lambda: self.speaker.write(0),
            "date",
            run_date=datetime.utcnow() + timedelta(seconds=1)
        )

    def sample_air(self):

        """
        Sample air.
        """

        return self.air.getSample()
