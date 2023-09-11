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
    namespace SceneGraphs
    {
        class SceneGraph
        {
        public:
            std::unordered_map<int, std::unqiue_ptr<Renderable>> renderables;
            
        };
    }
}