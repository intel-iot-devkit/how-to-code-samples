from __future__ import print_function

from traceback import print_exc

from requests import put as put_http
from requests.exceptions import RequestException

SERVER = "SERVER"
AUTH_TOKEN = "AUTH_TOKEN"

def store_message(config, payload):
    
    if not { SERVER, AUTH_TOKEN } <= set(config): return

    server = config[SERVER]
    auth_token = config[AUTH_TOKEN]

    headers = {
        "X-Auth-Token": auth_token
    }

    try:
        response = put_http(server, headers=headers, json=payload)
        response.raise_for_status()
        print("saved to data store")
    except RequestException:
        print("error saving to data store")
        print_exc()