/*
 * Twilio REST C++
 * Author: Laurent Luce
*/

#ifndef TWILIOUTILS_H
#define TWILIOUTILS_H

namespace twilio
{
  /** struct to contain the key and value for URL attributes or Body data */
  struct Var
  { string key;
    string value;
    Var(const string& k, const string& v)
    {
      key = k;
      value = v;
    }
  };
  typedef struct Var Var;

  class Utils {
    public:
      /**
       * Constructor
       */
      Utils(const string& id, const string& token)
      {
         tid = id;
         ttoken = token;
      }
      /**
        * Check if expectedSignature matches url + params base64 encoded hash
        * @param expectedSignature signature to compare to 
        * @param url URL to build hash on
        * @param params POST params
      */
      bool validateRequest(const string& expectedSignature, const string& url, vector<Var>& params);

    private:
      /** Twilio account SID */
      string tid;
      /** Twilio account token */
      string ttoken;
  };

  /**
    * Replace all occurences of needle in haystack by replacement
    * @param haystack string to search in
    * @param needle string to search for
    * @param replacement string to replace with
  */
  string replaceAll(const string& haystack, const string& needle, const string& replacement);

  /**
   * Convert integer to string
   * @param i integer to convert
   * @return string
  */
  string intToStr(int i);
  
  /**
   * '<' operator to compare 2 Var structure based on their key string
   * @param a Var 1
   * @param b Var 1
   * @return < result
  */
  bool operator<(const Var& a, const Var& b);
  
  /**
   * base64 encoding of input
   * @param input string to encode
   * @param length of string
   * @return encoded string
  */
  char *base64(const unsigned char *input, int length);

}

#endif
