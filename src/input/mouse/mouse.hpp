#pragma once

#include <map>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>


namespace Entropy::Input {
    struct Mouse {
        // Position of cursor
        glm::vec2 pos{};

        // Scrolling
        glm::vec2 scroll{};

        // Buttons
        std::map<int, bool> mouseDown;
    };
}

