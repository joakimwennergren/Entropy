#include "keyboard.hpp"

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_WINDOWS) || defined(BUILD_FOR_LINUX)

using namespace Entropy::Input;

Keyboard::Keyboard(std::shared_ptr<ServiceLocator> serviceLocator)
{
}

#endif