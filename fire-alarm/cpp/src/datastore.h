/*
* Copyright (c) 2015 - 2016 Intel Corporation.
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
* LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
* OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
* WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef DATASTORE_H_
#define DATASTORE_H_

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include "../lib/restclient-cpp/include/restclient-cpp/restclient.h"

/* Call nodejs/redis datastore to log data record
 *
 * Uses the following required ENV vars:
 *  SERVER - protocol, host name, and port of the remote server
 *  AUTH_TOKEN - token string to use for authentication against the remote server
 */
void log_datastore(std::string payload);

/* Call nodejs/redis datastore to increment counter
 *
 * Uses the following required ENV vars:
 *  SERVER - protocol, host name, and port of the remote server
 *  AUTH_TOKEN - token string to use for authentication against the remote server
 */
void increment_datastore();

#endif /* DATASTORE_H_ */
