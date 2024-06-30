#pragma once

#include <config.hpp>

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_WINDOWS) || defined(BUILD_FOR_LINUX)
#include <GLFW/glfw3.h>


#include "spdlog/spdlog.h"



namespace Entropy
{
    namespace Input
    {
        class Mouse
        {
        public:
            Mouse(GLFWwindow *window);

            // inline Vec2 Position()
            // {
            //     double xpos, ypos;
            //     glfwGetCursorPos(_window, &xpos, &ypos);
            //     // return Vec2{(float)xpos, (float)ypos};
            // }

        private:
            GLFWwindow *_window;
        };
    }
}
#endif