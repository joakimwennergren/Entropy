#pragma once

#include <glm/glm.hpp>
#include "node.hpp"

namespace Entropy
{
    namespace GLTF
    {
        struct Skin
        {
            std::string name;
            Node *skeletonRoot = nullptr;
            std::vector<glm::mat4> inverseBindMatrices;
            std::vector<Node *> joints;
        };

    }
}