// The program is using the `superagent` module
// to make the remote calls to the data store
var request = require("superagent");

// Store record in the remote datastore
exports.log = function(config, payload) {
  if (!config.SERVER || !config.AUTH_TOKEN) {
    return;
  }

  function callback(err, res) {
    if (err) {
      return console.error("err:", err);
    } else {
      return console.log("Saved to datastore:", payload);
    }
  }

  request
    .put(config.SERVER)
    .set("X-Auth-Token", config.AUTH_TOKEN)
    .send(payload)
    .end(callback);
};

exports.increment = function(config) {
  if (!config.SERVER || !config.AUTH_TOKEN) {
    return;
  }

  function callback(err, res) {
    if (err) {
      return console.error("err:", err);
    } else {
      console.log("Saved to datastore");
      return console.log("total count:", res.body.value);
    }
  }

  request
    .get(config.SERVER)
    .set("X-Auth-Token", config.AUTH_TOKEN)
    .end(callback);
};
