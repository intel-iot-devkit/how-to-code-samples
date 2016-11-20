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

from constants.hardware import GPS_DATA_RECEIVED, OBJECT_DETECTED

from log import log

class Reporter(object):

    def __init__(self, config, board):

        self.config = config
        self.board = board

        self.last_known_location = "Unknown"

        self.board.add_event_handler(GPS_DATA_RECEIVED, self.update_gps)
        self.board.add_event_handler(OBJECT_DETECTED, self.report_close_call)

    def update_gps(self, data):

        if data.sentence_type == "GGA":
            self.last_known_location = str(data)

    def report_close_call(self):

        """
        Report close call.
        """

        print("close call at", self.last_known_location)
        log(self.config, self.last_known_location)
