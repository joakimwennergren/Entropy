#pragma once
#include <config.hpp>
#include <services/service.hpp>
#include <servicelocators/servicelocator.hpp>
#include "spdlog/spdlog.h"

using namespace Entropy::Services;
using namespace Entropy::ServiceLocators;

namespace Entropy
{
    namespace Input
    {
        class Keyboard : public Service
        {
        public:
            Keyboard(std::shared_ptr<ServiceLocator> serviceLocator);
            bool _w;
            bool _a;
            bool _s;
            bool _d;

        private:
        };
    }
}