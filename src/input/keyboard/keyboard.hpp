#pragma once
#include <config.hpp>

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_WINDOWS) || defined(BUILD_FOR_LINUX)
#include <GLFW/glfw3.h>
#include <services/service.hpp>
#include <data/vectors.hpp>
#include <servicelocators/servicelocator.hpp>
#include "spdlog/spdlog.h"

using namespace Entropy::Services;
using namespace Entropy::Vectors;
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
#endif