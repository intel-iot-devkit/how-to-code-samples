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
from abc import ABCMeta, abstractmethod
from event_emitter import EventEmitter
from ..scheduler import SCHEDULER, ms

class PinMappings(dict):

    """
    Class for holding and manipulating pin mappings.
    """

    def __init__(self, *args, **kwargs):

        super(PinMappings, self).__init__(*args, **kwargs)
        self.__dict__ = self

    def __iadd__(self, value):
        for k in self.iterkeys():
            self[k] += value
        return self

    def __isub__(self, value):
        for k in self.iterkeys():
            self[k] -= value
        return self

class Board(object):

    """
    Base class for Board hardware implementations
    """

    __metaclass__ = ABCMeta

    def __init__(self):

        self.emitter = EventEmitter()

        self.hardware_event_job = SCHEDULER.add_job(
            self.update_hardware_state,
            "interval",
            seconds=ms(100),
            coalesce=True,
            max_instances=1)

    def trigger_hardware_event(self, event, *args, **kwargs):

        """
        Signal hardware event.
        """

        self.emitter.emit(event, *args, **kwargs)

    def add_event_handler(self, event, handler, once=False):

        """
        Add hardware event handler.
        """

        if once:
            self.emitter.once(event, handler)
        else:
            self.emitter.on(event, handler)

    def remove_event_handler(self, event, handler):

        """
        Remove hardware event handler.
        """

        self.emitter.remove(event, handler)

    @abstractmethod
    def update_hardware_state(self):

        """
        Abstract method for updating hardware state.
        """

        pass
