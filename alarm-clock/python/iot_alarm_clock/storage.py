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
from requests import put as put_http, get as get_http
from .config import DATA_STORE_CONFIG
from .scheduler import SCHEDULER

def store_message(payload, method="PUT"):

    """
    Publish message to remote data store.
    """

    if not DATA_STORE_CONFIG:
        return

    server = DATA_STORE_CONFIG.server
    auth_token = DATA_STORE_CONFIG.auth_token

    headers = {
        "X-Auth-Token": auth_token
    }

    def perform_request():

        """
        Perform HTTP request.
        """

        if method == "GET":
            response = get_http(
                server,
                headers=headers,
                json=payload
            )
        else:
            response = put_http(
                server,
                headers=headers,
                json=payload
            )
        response.raise_for_status()

        print("saved to data store")

    SCHEDULER.add_job(perform_request)
    