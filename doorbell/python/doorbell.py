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

from constants.hardware import TOUCH_DOWN, TOUCH_UP

class Doorbell(object):

    def __init__(self, config, board):

        self.config = config
        self.board = board

        self.board.add_event_handler(TOUCH_DOWN, self.ding_dong)
        self.board.add_event_handler(TOUCH_UP, self.reset)

        self.reset()

    def ding_dong(self):
        self.board.write_message("ding dong!")
        self.board.change_background("green")
        self.board.start_buzzer()

    def reset(self):
        self.board.write_message("doorbot ready")  
        self.board.change_background("white")
        self.board.stop_buzzer()