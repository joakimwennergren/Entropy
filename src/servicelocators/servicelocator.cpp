#include "servicelocator.hpp"

using namespace Entropy::ServiceLocators;

void ServiceLocator::registerService(const std::string &serviceName, std::shared_ptr<Service> service)
{
    services[serviceName] = service;
}

std::shared_ptr<Service> ServiceLocator::getService(const std::string &serviceName)
{
    return services[serviceName];
}