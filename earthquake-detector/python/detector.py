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

from time import sleep

from constants.hardware import ACCELERATION_DETECTED

from usgs import verify_earthquake

class Detector(object):

    def __init__(self, config, board):

        self.config = config
        self.board = board

        self.reset()
        self.board.add_event_handler(ACCELERATION_DETECTED, self.detect_earthquake)
    
    def display_verifying(self):

        self.board.write_message("Checking USGS...")
        self.board.change_background("blue")

    def display_warning(self):

        self.board.write_message("Earthquake!")
        self.board.change_background("red")
    
    def display_false_alarm(self):

        self.board.write_message("No quake.")
        self.board.change_background("green")

    def reset(self):

        self.board.write_message("Listening...")
        self.board.change_background("white")
    
    def detect_earthquake(self):

        """
        Detect earthquake with USGS verification.
        """

        self.display_verifying()
        verification = verify_earthquake(self.config)
        if (verification == True):
            self.display_warning()
        else:
            self.display_false_alarm()
        sleep(15)
        self.reset()
