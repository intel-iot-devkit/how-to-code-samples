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

from constants.hardware import SOUND_MEASUREMENT

from log import log

class Detector(object):

    def __init__(self, config, board):

        self.config = config
        self.board = board

        self.board.add_event_handler(SOUND_MEASUREMENT, self.display_sound_measurement)

    def display_sound_measurement(self, volume):

        """
        Display average volume measurement.
        """

        log(self.config, volume)

        self.board.write_message("avg volume: " + str(volume))

        if volume < 1:
            self.board.change_background("white")
        elif volume < 2:
            self.board.change_background("yellow")
        elif volume < 3:
            self.board.change_background("green")
        elif volume < 4:
            self.board.change_background("blue")
        elif volume < 5:
            self.board.change_background("purple")
        else:
            self.board.change_background("red")
