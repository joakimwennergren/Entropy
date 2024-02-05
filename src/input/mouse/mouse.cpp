#include "mouse.hpp"

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_WINDOWS) || defined(BUILD_FOR_LINUX)

using namespace Entropy::Input;

Mouse::Mouse(GLFWwindow *window)
{
    _window = window;
}

#endif