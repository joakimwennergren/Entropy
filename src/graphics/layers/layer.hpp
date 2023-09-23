/**
 * @file easing.hpp
 * @author Joakim Wennergren (joakim.wennergren@databeams.se)
 * @brief
 * @version 0.1
 * @date 2023-08-22
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include <string>
#include <unordered_map>
#include "renderable.hpp"


namespace Symbios
{
    namespace Layers
    {
        class Layer2D
        {
        public:
            Renderable * parent;
            Renderable * children;
        };
    }
}