#pragma once

#include <vulkan/vulkan.hpp>
#include <iostream>
#include <config.hpp>

#include <graphics/validationlayers/validationlayer.hpp>

using namespace Entropy::Graphics::ValidationLayers;

namespace Entropy
{
    namespace Graphics
    {
        namespace Instances
        {
            class VulkanInstance
            {
            public:
                VulkanInstance(std::string applicationName);
                inline VkInstance Get() { return _instance; };

            private:
                const std::vector<const char *> _validationLayers = {
                    "VK_LAYER_KHRONOS_validation"};

                VkInstance _instance;
            };
        }
    }
}
