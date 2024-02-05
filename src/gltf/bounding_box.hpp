#pragma once

#include <glm/glm.hpp>

namespace Entropy
{
    namespace GLTF
    {
        struct BoundingBox
        {
            glm::vec3 min;
            glm::vec3 max;
            bool valid = false;
            BoundingBox();
            BoundingBox(glm::vec3 min, glm::vec3 max);
            BoundingBox getAABB(glm::mat4 m);
        };
    }
}