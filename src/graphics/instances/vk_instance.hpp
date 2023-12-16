#pragma once

#include <vulkan/vulkan.hpp>
#include <iostream>
#include <config.hpp>

#include <graphics/validationlayers/validationlayer.hpp>
#include <services/service.hpp>

using namespace Entropy::Graphics::ValidationLayers;
using namespace Entropy::Services;

namespace Entropy
{
    namespace Graphics
    {
        namespace Instances
        {
            class VulkanInstance : public Service
            {
            public:
                VulkanInstance(std::string applicationName);
                inline VkInstance Get() { return _instance; };
                inline bool isValid() { return _instance != nullptr; };

            private:
                const std::vector<const char *> _validationLayers = {
                    "VK_LAYER_KHRONOS_validation"};

                VkInstance _instance;
            };
        }
    }
}
