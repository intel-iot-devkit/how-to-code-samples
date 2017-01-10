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
from collections import namedtuple
from upm.pyupm_i2clcd import Jhd1313m1
from upm.pyupm_uln200xa import ULN200XA, ULN200XA_DIR_CW as DIR_CW, ULN200XA_DIR_CCW as DIR_CCW
from upm.pyupm_joystick12 import Joystick12
from mraa import addSubplatform, GENERIC_FIRMATA
from ..config import HARDWARE_CONFIG, KNOWN_PLATFORMS
from .board import Board, PinMappings
from .events import JOYSTICK_READING

StepDirection = namedtuple("StepDirection", "cw ccw")
STEP_DIRECTION = StepDirection(
    cw=0x01,
    ccw=0x02
)

JoyStickReading = namedtuple("JoyStickReading", "x y")

class GroveBoard(Board):

    """
    Board class for Grove hardware.
    """

    def __init__(self):

        super(GroveBoard, self).__init__()

        # pin mappings
        self.pin_mappings = PinMappings(
            joy_x_pin=2,
            joy_y_pin=3,
            step1_in1_pin=9,
            step1_in2_pin=10,
            step1_in3_pin=11,
            step1_in4_pin=12,
            step2_in1_pin=4,
            step2_in2_pin=5,
            step2_in3_pin=6,
            step2_in4_pin=7,
            i2c_bus=6
        )

        self.voltage = 1.0
        if HARDWARE_CONFIG.platform == KNOWN_PLATFORMS.firmata:
            addSubplatform(GENERIC_FIRMATA, "/dev/ttyACM0")
            self.pin_mappings += 512
            self.pin_mappings.i2c_bus = 512
            self.voltage = 1.33

        self.screen = Jhd1313m1(self.pin_mappings.i2c_bus, 0x3E, 0x62)
        self.joy = Joystick12(self.pin_mappings.joy_x_pin, self.pin_mappings.joy_y_pin)

        self.step1 = ULN200XA(
            4096,
            self.pin_mappings.step1_in1_pin,
            self.pin_mappings.step1_in2_pin,
            self.pin_mappings.step1_in3_pin,
            self.pin_mappings.step1_in4_pin
        )

        self.step2 = ULN200XA(
            4096,
            self.pin_mappings.step2_in1_pin,
            self.pin_mappings.step2_in2_pin,
            self.pin_mappings.step2_in3_pin,
            self.pin_mappings.step2_in4_pin
        )


    def update_hardware_state(self):

        """
        Update hardware state.
        """

        joystick_reading = self.read_joystick()
        self.trigger_hardware_event(JOYSTICK_READING, joystick_reading)

    # hardware functions
    def move(self, step_id, direction):

        if step_id == 1:
            self.step1.setSpeed(5)
            self.step1.setDirection(DIR_CW if direction == STEP_DIRECTION.cw else DIR_CCW)
            self.step1.stepperSteps(1020)

        if step_id == 2:
            self.step2.setSpeed(5)
            self.step2.setDirection(DIR_CW if direction == STEP_DIRECTION.cw else DIR_CCW)
            self.step2.stepperSteps(1020)

    def read_joystick(self):

        x_input = self.joy.getXInput() * self.voltage
        y_input = self.joy.getYInput() * self.voltage
        return JoyStickReading(x=x_input, y=y_input)

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
