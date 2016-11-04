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

from traceback import print_exc

from requests import put as put_http
from requests.exceptions import RequestException

from constants.config import SERVER, AUTH_TOKEN

from scheduler import scheduler

def store_message(config, payload):

    """
    Publish message to remote data store.
    """
    
    if not { SERVER, AUTH_TOKEN } <= set(config): return

    server = config[SERVER]
    auth_token = config[AUTH_TOKEN]

    headers = {
        "X-Auth-Token": auth_token
    }
    
    def perform_request(): 
        response = put_http(server, headers=headers, json=payload)
        response.raise_for_status()
        print("saved to data store")

    scheduler.add_job(perform_request)
    