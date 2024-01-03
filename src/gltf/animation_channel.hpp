#pragma once

#include <glm/glm.hpp>
#include "node.hpp"

namespace Entropy
{
    namespace GLTF
    {
        struct AnimationChannel
        {
            enum PathType
            {
                TRANSLATION,
                ROTATION,
                SCALE
            };
            PathType path;
            Node *node;
            uint32_t samplerIndex;
        };
    }
}