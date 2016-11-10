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

from mraa import Gpio, DIR_OUT

from pyupm_i2clcd import SAINSMARTKS
from pyupm_grove import GroveButton

from constants.hardware import TOUCH_DOWN, TOUCH_UP

from board import Board

class DfrobotBoard(Board):

    """
    Board class for drobot hardware.
    """

    def __init__(self, config):

        super(GroveBoard, self).__init__()

        self.touch_pin = 16
        self.buzzer_pin = 15
        
        self.touch = GroveButton(self.touch_pin)
        self.buzzer = Gpio(self.buzzer_pin)
        self.screen = Jhd1313m1(8, 9, 4, 5, 6, 7, 0)

        self.buzzer.dir(DIR_OUT)
    
    def update_hardware_state(self):

        """
        Update hardware state.
        """

        current_touch_state = self.detect_touch()
        if (self.touch_state != current_touch_state):

            if (current_touch_state == True):
                self.trigger_hardware_event(TOUCH_DOWN)
            else:
                self.trigger_hardware_event(TOUCH_UP)

            self.touch_state = current_touch_state

    # hardware functions
    def detect_touch(self):

        """
        Detect touch state.
        """

        return self.touch.value()

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
        No effect on the dfrobot.
        """
        
        pass
