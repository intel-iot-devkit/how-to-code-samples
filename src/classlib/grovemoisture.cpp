#include <iostream>
#include <string>
#include <stdexcept>

#include "grovemoisture.h"

using namespace upm;

GroveMoisture::GroveMoisture(int pin)
{
    if (!(m_aio = mraa_aio_init(pin)))
        throw std::invalid_argument(std::string(__FUNCTION__) +
            ": mraa_aio_init() failed, invalid pin?");
}

GroveMoisture::~GroveMoisture()
{
    mraa_aio_close(m_aio);
}

int GroveMoisture::value()
{
    return mraa_aio_read(m_aio);
}