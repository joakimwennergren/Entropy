#pragma once

#include <glm/glm.hpp>

namespace Entropy
{
    namespace GLTF
    {
        struct AnimationSampler
        {
            enum InterpolationType
            {
                LINEAR,
                STEP,
                CUBICSPLINE
            };
            InterpolationType interpolation;
            std::vector<float> inputs;
            std::vector<glm::vec4> outputsVec4;
        };
    }
}