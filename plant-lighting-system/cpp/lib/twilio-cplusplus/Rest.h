/*
 * Twilio REST C++
 * Author: Laurent Luce
*/

#ifndef TWILIOREST_H
#define TWILIOREST_H

namespace twilio {

  class Rest {
    public:
      /**
       * Constructor
       * @param id Twilio account SID
       * @param token Twilio account token
       */
      Rest(const string& id, const string& token)
      {
         tid = id;
         ttoken = token;
      }

      /**
       * HTTP request
       * @param path URL path
       * @param method HTTP method: "GET", "POST", "PUT", "DELETE"
       * @param vars URL attributes or POST attributes
       * @return HTTP response
       */
      string request(const string& path, const string& method, const vector<Var>& vars);

    private:
      /** Twilio account SID */
      string tid;
      /** Twilio account token */
      string ttoken;
      /** Response buffer */
      string tbuffer;
      /** Twilio API URL - set in cpp */
      static const string TWILIO_API_URL;

      /**
       * Prefix path with Twilio URL
       * @param path URL path
       * @return full URL
       */
      string build_uri(const string& path) const;
      /**
       * HTTP GET request
       * @param url HTTP request URL
       * @param vars URL attributes
       * @return HTTP response
       */
      string get(const string& url, const vector<Var>& vars);
      /**
       * HTTP POST request
       * @param url HTTP request URL
       * @param vars POST attributes
       * @return HTTP response
       */
      string post(const string& url, const vector<Var>& vars);
      /**
       * HTTP PUT request
       * @param url HTTP request URL
       * @param filename File to read data from
       * @return HTTP response
      */
      string put(const string& url, const string& filename);
      /**
       * HTTP DELETE request
       * @param url HTTP request URL
       * @return HTTP response
      */
      string tdelete(const string& url);

  };

}

#endif
