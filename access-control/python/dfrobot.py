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

from pyupm_i2clcd import SAINSMARTKS
from pyupm_grove import GroveButton, GroveRotary

from events import scheduler, emitter, ms
from board import Board

class DfrobotBoard(Board):

    """
    Board class for drobot hardware.
    """

    def __init__(self, config):

        super(GroveBoard, self).__init__()
        
        # pin mappings
        self.motion_pin = 16
        
        self.motion = BISS0001(self.motion_pin)
        self.screen = Jhd1313m1(8, 9, 4, 5, 6, 7, 0)

        self.last_motion = False
    
    def update_hardware_state(self):

        """
        Update hardware state.
        """

        # detect motion change
        current_motion = self.detect_motion()
        if current_motion != self.last_motion:
            if current_motion:
                self.trigger_hardware_event(MOTION_DETECTED)
            
            self.last_motion = current_motion

    # hardware functions
    def detect_motion(self):

        """
        Check PIR motion sensor.
        """

        return self.motion.value()

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
