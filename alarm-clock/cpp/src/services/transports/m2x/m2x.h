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

#ifndef SERVICES_TRANSPORTS_M2X_M2X_H_INCLUDED
#define SERVICES_TRANSPORTS_M2X_M2X_H_INCLUDED

#include <string>
#include <iostream>
#include <utility>
#include <ctime>

#include "../../../../lib/restclient-cpp/include/restclient-cpp/restclient.h"
#include "../../../../lib/jsoncpp/json/json.h"

#include "../../util.h"

struct m2x_service_config
{
    std::string api_key;
    std::string device_id;
    std::string stream_id;
};

std::pair<m2x_service_config, bool> get_m2x_config();
void m2x_publish(std::string const &message);

#endif