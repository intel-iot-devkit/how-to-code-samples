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
from upm.pyupm_mma7660 import MMA7660, new_floatp as accel_float, floatp_value as accel_value
from mraa import addSubplatform, GENERIC_FIRMATA
from ..config import HARDWARE_CONFIG, KNOWN_PLATFORMS
from .board import Board, PinMappings
from .events import ACCELERATION_DETECTED

class GroveBoard(Board):

    """
    Board class for Grove hardware.
    """

    def __init__(self):

        super(GroveBoard, self).__init__()

        # pin mappings
        self.pin_mappings = PinMappings(
            i2c_bus=6
        )

        if HARDWARE_CONFIG.platform == KNOWN_PLATFORMS.firmata:
            addSubplatform(GENERIC_FIRMATA, "/dev/ttyACM0")
            self.pin_mappings += 512
            self.pin_mappings.i2c_bus = 512

        self.screen = Jhd1313m1(self.pin_mappings.i2c_bus, 0x3E, 0x62)
        self.accelerometer = MMA7660(self.pin_mappings.i2c_bus, 0x4C)

        # accelerometer setup
        self.ax = accel_float()
        self.ay = accel_float()
        self.az = accel_float()

        self.accelerometer.setModeStandby()
        self.accelerometer.setSampleRate(0x01)
        self.accelerometer.setModeActive()

        self.acceleration_detected = False


    def update_hardware_state(self):

        """
        Update hardware state.
        """

        current_acceleration = self.detect_acceleration()
        if current_acceleration != self.acceleration_detected:
            if current_acceleration:
                self.trigger_hardware_event(ACCELERATION_DETECTED)
            self.acceleration_detected = current_acceleration

    # hardware functions
    def detect_acceleration(self):

        """
        Detect acceleration change.
        """

        self.accelerometer.getAcceleration(self.ax, self.ay, self.az)

        ax = accel_value(self.ax)
        ay = accel_value(self.ay)
        az = accel_value(self.az)

        if ax > 1.2 or ay > 1.2 or az > 1.2:
            return True
        else:
            return False

    def write_message(self, message, line=0):

        """
        Write message to LCD screen.
        """

        message = message.ljust(16)
        self.screen.setCursor(line, 0)
        self.screen.write(message)
        print(message)

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
