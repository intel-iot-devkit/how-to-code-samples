/*
 * Twilio REST C++
 * Author: Laurent Luce
*/

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <curl/curl.h>
#if LIBCURL_VERSION_NUM < 0x071507
#include <curl/types.h>
#endif
#include <curl/easy.h>

using namespace std;

#include "Utils.h"
#include "Rest.h"
using namespace twilio;

/**
 * HTTP request
 * @param path URL path
 * @param method HTTP method: "GET", "POST", "PUT", "DELETE"
 * @param vars URL attributes or POST attributes
 * @return HTTP response
 */
string Rest::request(const string& path, const string& method, const vector<Var>& vars)
{
  string response, m;

  if (path.length() <= 0)
    throw "Path is empty";

  m = method;

  transform(m.begin(), m.end(), m.begin(), ::toupper);

  if ((m != "GET" && m != "POST"
        && m != "PUT" && m != "DELETE"))
  {
    throw "Invalid m parameter";
  }
    
  string url = build_uri(path);
  if(m == "GET")
  {
    response = get(url, vars);
  }
  else if(m == "POST")
  {
    response = post(url, vars);
  }
  else if(m == "PUT")
  {
    response = put(url, vars[0].value);
  }
  else if(m == "DELETE")
  {
    response = tdelete(url);
  }
  
  return response;
}

/**
 * Curl write function
 * @param data data received from server
 * @param size data size is size * nmemb
 * @param nmemb data size is size * nmemb
 * @param buffer
 */
static int writer(char *data, size_t size, size_t nmemb, string *buffer)
{
  int result = 0;
  if (buffer != NULL)
  {
    buffer->append(data, size * nmemb);
    result = size * nmemb;
  }
  return result;
}

/**
 * Curl read callback function
 * @param ptr pointer to storage 
 * @param size data size is size * nmemb
 * @param nmemb data size is size * nmemb
 * @param userdata read from stream
 */
static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t retcode;
     
  retcode = fread((FILE*)ptr, size, nmemb, (FILE*)stream);
         
  return retcode;
}

/**
 * HTTP GET request
 * @param url HTTP request URL
 * @param vars URL attributes or POST attributes
 * @return HTTP response
 */
string Rest::get(const string& url, const vector<Var>& vars)
{
  string query = "";
  string u;
  
  for(unsigned int i = 0; i < vars.size(); i++)
  {
    query += "&" + vars[i].key + "=" + vars[i].value;
  }
    
  if (query.length() > 0)
    u = url + "?" + query.substr(1);

  CURL *curl;
  CURLcode res;
     
  tbuffer = "HTTP get error";
  curl = curl_easy_init();
  if(curl) {
    //url = curl_easy_escape(curl, url.c_str(), url.length());
    string sAuth = tid + ":" + ttoken;
    curl_easy_setopt(curl, CURLOPT_URL, u.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
    curl_easy_setopt(curl, CURLOPT_USERPWD, sAuth.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
    tbuffer = "";
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &tbuffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if (res == CURLE_OK)
      return tbuffer;
  }
  
  return tbuffer;
}

/**
 * HTTP POST request
 * @param url HTTP request URL
 * @param vars POST attributes
 * @return HTTP response
*/
string Rest::post(const string& url, const vector<Var>& vars)
{
  CURL *curl;
  CURLcode res;
    
  struct curl_httppost *formpost = NULL;
  struct curl_httppost *lastptr = NULL;
  struct curl_slist *headerlist = NULL;
  static const char buf[] = "Expect:";

  curl_global_init(CURL_GLOBAL_ALL);

  // Post data
  for(unsigned int i = 0; i < vars.size(); i++)
  {
    curl_formadd(&formpost,
                 &lastptr,
                 CURLFORM_COPYNAME, vars[i].key.c_str(),
                 CURLFORM_COPYCONTENTS, vars[i].value.c_str(),
                 CURLFORM_END);
  }

  tbuffer = "HTTP post error";
  curl = curl_easy_init();
  headerlist = curl_slist_append(headerlist, buf);

  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    string sAuth = tid + ":" + ttoken;
    curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
    curl_easy_setopt(curl, CURLOPT_USERPWD, sAuth.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
    tbuffer = "";
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &tbuffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    curl_formfree(formpost);
    curl_slist_free_all (headerlist);
    curl_global_cleanup();
    if (res == CURLE_OK)
      return tbuffer;
  }
  
  return tbuffer;
}

/**
 * HTTP PUT request
 * @param url HTTP request URL
 * @param filename File to read data from
 * @return HTTP response
*/
string Rest::put(const string& url, const string& filename)
{
  CURL *curl;
  CURLcode res;
  FILE * hd_src ;
  int hd ;
  struct stat file_info;
  
  // get local file size 
  hd = open(filename.c_str(), O_RDONLY);
  fstat(hd, &file_info);
  close(hd);
  hd_src = fopen(filename.c_str(), "rb");

  curl_global_init(CURL_GLOBAL_ALL);

  tbuffer = "HTTP put error";
  curl = curl_easy_init();

  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    string sAuth = tid + ":" + ttoken;
    curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
    curl_easy_setopt(curl, CURLOPT_USERPWD, sAuth.c_str());
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);
    curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, 
                      (curl_off_t)file_info.st_size);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
    tbuffer = "";
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &tbuffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    if (res == CURLE_OK) {
      fclose(hd_src);
      return tbuffer;
    }
  }
  
  fclose(hd_src);
  return tbuffer;
}

/**
 * HTTP DELETE request
 * @param url HTTP request URL
 * @return HTTP response
*/
string Rest::tdelete(const string& url)
{
  CURL *curl;
  CURLcode res;
    
  tbuffer = "HTTP delete error";
  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
    string sAuth = tid + ":" + ttoken;
    curl_easy_setopt(curl, CURLOPT_USERPWD, sAuth.c_str());
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
    tbuffer = "";
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &tbuffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if (res == CURLE_OK)
      return tbuffer;
  }
  
  return tbuffer;
}

/**
 * Prefix path with Twilio URL
 * @param path URL path
 * @return full URL
 */
string Rest::build_uri(const string& path) const
{
  if (path[0] == '/')
    return TWILIO_API_URL + path;
  else
    return TWILIO_API_URL + "/" + path;
}
 
const string Rest::TWILIO_API_URL = "https://api.twilio.com";


