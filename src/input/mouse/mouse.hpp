#pragma once

#include <map>
#include <config.hpp>
#include <spdlog/spdlog.h>

namespace Entropy
{
    namespace Input
    {
        struct Mouse
        {
            // Position of cursor
            float xPos;
            float yPos;

            // Scrolling
            float xOffset;
            float yOffset;

            // Buttons
            std::map<int, bool> mouseDown;
        };
    }
}