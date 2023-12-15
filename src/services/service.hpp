#pragma once

#include <config.hpp>

namespace Entropy
{
    namespace Services
    {
        class Service
        {
        public:
            virtual ~Service() = default;
            virtual bool isValid() = 0;
        };
    }
}
