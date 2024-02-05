#include "script.hpp"

using namespace Entropy::Scripting;

Script::Script(std::shared_ptr<ServiceLocator> serviceLocator)
{
    _serviceLocator = serviceLocator;
}