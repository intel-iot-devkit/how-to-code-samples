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

from datetime import datetime, timedelta

from requests import get as get_http

from scheduler import scheduler

def verify_earthquake(config):

    """
    Verify earthquake using USGS service.
    """
    
    if not { LATITUDE, LONGITUDE } <= set(config): return False

    geo_lat = config[LATITUDE]
    geo_long = config[LONGITUDE]

    server = "http://earthquake.usgs.gov/fdsnws/event/1/query"

    time_window = datetime.utcnow() - timedelta(minutes=10)

    query = {
        "format": "getjson",
        "starttime": time_window.isoformat(),
        "latitude": geo_lat,
        "longitude": geo_long,
        "maxradiuskm": 500
    }
    
    print("calling USGS service")
    response = get_http(server, params=query)
    response.raise_for_status()
    data = response.json()
    event_count = len(data["features"])
    return True if event_count > 0 else False
    