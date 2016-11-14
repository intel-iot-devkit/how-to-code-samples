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


from mraa import Gpio
from mraa import Aio
from pyupm_i2clcd import SAINSMARTKS

from scheduler import scheduler, ms
from board import Board

class DfrobotBoard(Board):

    """
    Board class for drobot hardware.
    """

    def __init__(self, config):
        
        # pin mappings
        self.temperature_pin = 1
        self.buzzer_pin = 16
        self.current = 0
        
        self.screen = SAINSMARTKS(8, 9, 4, 5, 6, 7, 0)
        self.buzzer = Gpio(self.buzzer_pin)
        self.temperature = Aio(self.temperature_pin)

    # hardware functions

    def get_temperature(self):

        """
        Get the temperature and convert to degrees celcius.
        """
        return  (500 * self.temperature.read()) / 1024
         

    def start_buzzer(self):

        """
        Start the hardware buzzer.
        """

        self.buzzer.write(1)

    def stop_buzzer(self):

        """
        Stop the hardware buzzer.
        """

        self.buzzer.write(10)

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
