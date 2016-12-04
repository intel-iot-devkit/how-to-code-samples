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
from pynmea2 import NMEAStreamReader, ParseError, ChecksumError
from upm.pyupm_nmea_gps import NMEAGPS
from upm.pyupm_rfr359f import RFR359F
from mraa import addSubplatform, GENERIC_FIRMATA
from ..config import HARDWARE_CONFIG, KNOWN_PLATFORMS
from .board import Board, PinMappings
from .events import GPS_DATA_RECEIVED, OBJECT_DETECTED

class GroveBoard(Board):

    """
    Board class for Grove hardware.
    """

    def __init__(self):

        super(GroveBoard, self).__init__()

        # pin mappings
        self.pin_mappings = PinMappings(
            interrupter_pin=2,
            uart_bus=0
        )

        if HARDWARE_CONFIG.platform == KNOWN_PLATFORMS.firmata:
            addSubplatform(GENERIC_FIRMATA, "/dev/ttyACM0")
            self.pin_mappings += 512

        self.gps = NMEAGPS(self.pin_mappings.uart_bus, 9600, -1)
        self.interrupter = RFR359F(self.pin_mappings.interrupter_pin)

        self.obj_detected_state = False
        self.nmea_stream_reader = NMEAStreamReader()

    def update_hardware_state(self):

        """
        Update hardware state.
        """

        # handle gps data

        gps_data = self.query_gps()
        if gps_data:
            for gps_msg in gps_data:
                self.trigger_hardware_event(GPS_DATA_RECEIVED, gps_msg)

        # handle IR interrupter data

        obj_detected = self.detect_object()
        if self.obj_detected_state != obj_detected:

            if obj_detected:
                self.trigger_hardware_event(OBJECT_DETECTED)

            self.obj_detected_state = obj_detected

    # hardware functions
    def query_gps(self):

        """
        Query GPS receiver.
        """

        print("Running GPS query.")
        if self.gps.dataAvailable(5000):

            try:
                payload = self.gps.readStr(256).decode("utf8", "ignore")
                data = self.nmea_stream_reader.next(payload)
            except (ParseError, ChecksumError):
                print("GPS result: (Error) No Data.")
                return None

            print("GPS result: {0} messages.".format(len(data)))
            return data
        else:
            print("GPS result: No Data.")
            return None

    def detect_object(self):

        """
        Detect object.
        """

        return self.interrupter.objectDetected()
