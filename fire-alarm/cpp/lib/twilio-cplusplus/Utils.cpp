/*
 * Twilio REST C++
 * Author: Laurent Luce
*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/sha.h>
#include <string.h>

using namespace std;

#include "Utils.h"
#include "Rest.h"

namespace twilio {
  /**
    * Check if expectedSignature matches url + params base64 encoded hash
    * @param expectedSignature signature to compare to 
    * @param url URL to build hash on
    * @param params POST params
  */
  bool Utils::validateRequest(const string& expectedSignature, const string& url, vector<Var>& params)
  {
    string u = url;

    // sort params alphabetically
    sort(params.begin(), params.end());
  
    // add params to url
    for(unsigned int i = 0; i < params.size(); i++)
    {
      u += params[i].key + params[i].value;
    }

    // HMAC SHA1 + base64 encoding
    unsigned int len = ttoken.length();
    unsigned char md[20];
    unsigned int md_len;
    HMAC(EVP_sha1(), ttoken.c_str(), (int)len, (unsigned char*)(u.c_str()), u.length(), md, &md_len);
    char* digest_enc;
    digest_enc = base64((unsigned char*)md, (int)md_len);
    cout << digest_enc << endl;

    // compare hash/encoded string with the expected string
    if(memcmp(expectedSignature.c_str(), digest_enc, strlen(digest_enc)) == 0)
      return true;
    else
      return false;
  }

  /**
    * Replace all occurences of needle in haystack by replacement
    * @param haystack string to search in
    * @param needle string to search for
    * @param replacement string to replace with
  */
  string replaceAll(const string& haystack, const string& needle, const string& replacement) {
    if (needle.empty()) {
      return haystack;
    }
    string result;
    for (string::const_iterator substart = haystack.begin(), subend; ; ) {
      subend = search(substart, haystack.end(), needle.begin(), needle.end());
      copy(substart, subend, back_inserter(result));
      if (subend == haystack.end()) {
        break;
      }
      copy(replacement.begin(), replacement.end(), back_inserter(result));
      substart = subend + needle.size();
    }
    return result;
  }

  /**
   * Convert integer to string
   * @param i integer to convert
   * @return string
  */
  string intToStr(int i)
  {
    stringstream out;
    out << i;
    return out.str();
  }
 
  /**
   * '<' operator to compare 2 Var structure based on their key string
   * @param a Var 1
   * @param b Var 2
   * @return < result
  */
  bool operator<(const Var& a, const Var& b) {
        return a.key < b.key;
  }

  /**
   * base64 encoding of input
   * @param input string to encode
   * @param length of string
   * @return encoded string
  */
  char *base64(const unsigned char *input, int length)
  {
    BIO *bmem, *b64;
    BUF_MEM *bptr;

    b64 = BIO_new(BIO_f_base64());
    bmem = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, bmem);
    BIO_write(b64, input, length);
    BIO_flush(b64);
    BIO_get_mem_ptr(b64, &bptr);

    char *buff = (char *)malloc(bptr->length);
    memcpy(buff, bptr->data, bptr->length-1);
    buff[bptr->length-1] = 0;

    BIO_free_all(b64);

    return buff;
  }
}



