#include "servicelocator.hpp"

using namespace Entropy::ServiceLocators;

// Register a service with the locator
void ServiceLocator::registerService(const std::string &serviceName, std::shared_ptr<Service> service)
{
    services[serviceName] = service;
}

// Get a service from the locator
std::shared_ptr<Service> ServiceLocator::getService(const std::string &serviceName)
{
    return services[serviceName];
}