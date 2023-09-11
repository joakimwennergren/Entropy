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
#include <glm/glm.hpp>


namespace Symbios
{
    namespace Renderables
    {
        class Renderable
        {
        public:
            glm::vec3 position = glm::vec3(0.0);
            glm::vec3 rotation = glm::vec3(1.0);
            glm::vec3 scale = glm::vec3(1.0);
            glm::vec4 color = glm::vec4(1.0);

            int textureId = -1;
        };
    }
}