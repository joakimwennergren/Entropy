#pragma once

#include <map>
#include <config.hpp>
#include <spdlog/spdlog.h>

namespace Entropy
{
    namespace Input
    {
        struct Keyboard
        {
            std::map<int, bool> keysDown;
            bool keyCtrl;
            bool keyShift;
            bool keyAlt;
            bool keySuper;
        };
    }
}