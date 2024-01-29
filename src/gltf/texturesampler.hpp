#pragma once

#include <vulkan/vulkan.hpp>

namespace Entropy
{
    namespace GLTF
    {
        struct TextureSampler
        {
            VkFilter magFilter;
            VkFilter minFilter;
            VkSamplerAddressMode addressModeU;
            VkSamplerAddressMode addressModeV;
            VkSamplerAddressMode addressModeW;
        };
    }
}