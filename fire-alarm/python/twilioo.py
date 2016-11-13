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

from twilio.rest import TwilioRestClient

from constants.config import TWILIO_ACCT_SID, TWILIO_AUTH_TOKEN, TWILIO_OUTBOUND_NUMBER, TWILIO_INBOUND_NUMBER

from scheduler import scheduler

def send_sms(config, payload):
 
    """
    Send SMS via Twilio.
    """

    if not { TWILIO_ACCT_SID, TWILIO_AUTH_TOKEN, TWILIO_OUTBOUND_NUMBER, TWILIO_INBOUND_NUMBER } <= set(config): return

    twilio_sid = config[TWILIO_ACCT_SID]
    twilio_auth = config[TWILIO_AUTH_TOKEN]
    twilio_outbound = config[TWILIO_OUTBOUND_NUMBER]
    twilio_inbound = config[TWILIO_INBOUND_NUMBER]

    client = TwilioRestClient(twilio_sid, twilio_auth)

    def perform_request():
        message = client.messages.create(body=payload,
            to=twilio_inbound,
            from_=twilio_outbound)

        print("sent SMS")

    scheduler.add_job(perform_request)
