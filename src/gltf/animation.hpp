#pragma once

#include <glm/glm.hpp>
#include "animation_sampler.hpp"
#include "animation_channel.hpp"

namespace Entropy
{
    namespace GLTF
    {
        struct Animation
        {
            std::string name;
            std::vector<AnimationSampler> samplers;
            std::vector<AnimationChannel> channels;
            float start = std::numeric_limits<float>::max();
            float end = std::numeric_limits<float>::min();
        };
    }
}