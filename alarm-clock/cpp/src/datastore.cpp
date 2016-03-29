
#include "datastore.h"

void log_datastore(std::string payload) {
  if (!getenv("SERVER") || !getenv("AUTH_TOKEN")) {
    return;
  }

  RestClient::headermap headers;
  headers["X-Auth-Token"] = getenv("AUTH_TOKEN");

  RestClient::response r = RestClient::put(getenv("SERVER"), "text/json", payload, headers);
  std::cout << "Datastore called. Result:" << r.code << std::endl;
  std::cout << r.body << std::endl;
}
