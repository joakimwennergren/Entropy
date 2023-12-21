#pragma once

#include <GLFW/glfw3.h>

#include <services/service.hpp>
#include <data/vectors.hpp>

#include "spdlog/spdlog.h"

using namespace Entropy::Services;
using namespace Entropy::Vectors;

namespace Entropy
{
    namespace Input
    {
        class Mouse : public Service
        {
        public:
            Mouse(GLFWwindow *window);
            inline bool isValid() override { return _window != nullptr; };

            inline Vec2 Position()
            {
                double xpos, ypos;
                glfwGetCursorPos(_window, &xpos, &ypos);
                return Vec2{(float)xpos, (float)ypos};
            }

        private:
            GLFWwindow *_window;
        };
    }

}
