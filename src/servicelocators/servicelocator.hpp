#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <services/service.hpp>

using namespace Entropy::Services;

namespace Entropy
{
    namespace ServiceLocators
    {
        class ServiceLocator
        {
        public:
            void registerService(const std::string &serviceName, std::shared_ptr<Service> service);
            std::shared_ptr<Service> getService(const std::string &serviceName);

        private:
            std::unordered_map<std::string, std::shared_ptr<Service>> services;
        };
    }
}
