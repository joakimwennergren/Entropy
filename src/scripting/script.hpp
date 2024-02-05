#pragma once

#include <string>

#include <servicelocators/servicelocator.hpp>

#include <sol/sol.hpp>

using namespace Entropy::ServiceLocators;

namespace Entropy
{
    namespace Scripting
    {
        class Script
        {
        public:
            Script() = default;
            Script(std::shared_ptr<ServiceLocator> serviceLocator);
            std::string script;
            std::string scriptFile;
            sol::environment environment;
            bool hasExecuted = false;

        private:
            std::shared_ptr<ServiceLocator>
                _serviceLocator;
        };
    }
}
