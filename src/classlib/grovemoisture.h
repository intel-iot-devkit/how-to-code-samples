#pragma once
#include <string>
#include <mraa/aio.h>

namespace upm {
      class GroveMoisture {
          public:
                GroveMoisture(int pin);
                ~GroveMoisture();
                int value();
           
                  private:
                        mraa_aio_context m_aio;
                   
    };

}