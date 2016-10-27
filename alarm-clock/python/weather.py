from requests import get as get_http, HTTPError

def get_weather(config):
    
    if not { "WEATHER_API_KEY", "LOCATION" } <= set(config): return

    api_key = config["WEATHER_API_KEY"]
    location = config["LOCATION"]

    url = "http://api.wunderground.com/api/{0}/conditions/q/CA/{1}.json".format(api_key, location)

    response = get_http(url)
    data = response.json()
    conditions = data["current_observation"]["weather"]

    return conditions.encode("ascii", "ignore")