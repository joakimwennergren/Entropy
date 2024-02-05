#pragma once

#include <vulkan/vulkan.hpp>
#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <config.hpp>

#include <graphics/devices/logical_device.hpp>
#include <services/service.hpp>

using namespace Entropy::Graphics::Surfaces;
using namespace Entropy::Graphics::QueueFamilies;
using namespace Entropy::Graphics::Devices;
using namespace Entropy::Services;

namespace Entropy
{
    namespace Graphics
    {
        namespace DescriptorsetLayouts
        {
            class DescriptorsetLayout : public Service
            {
            public:
                DescriptorsetLayout(std::shared_ptr<LogicalDevice> logicalDevice, std::vector<VkDescriptorSetLayoutBinding> layoutBindings);
                inline VkDescriptorSetLayout Get() { return _descriptorSetLayout; };
                inline bool isValid() { return true; };

            private:
                VkDescriptorSetLayout _descriptorSetLayout;
            };
        }
    }
}
