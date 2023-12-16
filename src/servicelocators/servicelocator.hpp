#pragma once

#include <iostream>
#include <unordered_map>
#include <config.hpp>

#include <services/service.hpp>

using namespace Entropy::Services;

namespace Entropy
{
    namespace ServiceLocators
    {
        class ServiceLocator
        {
        public:
            // Register a service with the locator
            void registerService(const std::string &serviceName, std::shared_ptr<Service> service);

            // Get a service from the locator
            std::shared_ptr<Service> getService(const std::string &serviceName);

        private:
            std::unordered_map<std::string, std::shared_ptr<Service>> services;
        };
    }
}
