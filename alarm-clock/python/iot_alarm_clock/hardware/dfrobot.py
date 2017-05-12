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
from upm.pyupm_rotary import Rotary
from mraa import Gpio, DIR_OUT, addSubplatform, GENERIC_FIRMATA
from ..config import HARDWARE_CONFIG, KNOWN_PLATFORMS
from .board import Board, PinMappings
from .events import ROTARY, BUTTON_RELEASED

class DfrobotBoard(Board):

    """
    Board class for drobot hardware.
    """

    def __init__(self):

        super(DfrobotBoard, self).__init__()

        # pin mappings
        self.pin_mappings = PinMappings(
            buzzer_pin=15,
            button_pin=16,
            rotary_pin=3,
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
        No effect on the dfrobot.
        """

        pass

    def change_brightness(self, value):

        """
        Change LCD screen brightness.
        Only supports on (value < 512) and off (value >= 512) on the dfrobot.
        """

        if value < 512:
            self.screen.displayOff()
        else:
            self.screen.displayOn()
