#pragma once

#include <vulkan/vulkan.hpp>
#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <config.hpp>

#include <graphics/devices/logical_device.hpp>

using namespace Entropy::Graphics::Surfaces;
using namespace Entropy::Graphics::QueueFamilies;
using namespace Entropy::Graphics::Devices;

namespace Entropy
{
    namespace Graphics
    {
        namespace DescriptorsetLayouts
        {
            class DescriptorsetLayout
            {
            public:
                DescriptorsetLayout(std::shared_ptr<LogicalDevice> logicalDevice, std::vector<VkDescriptorSetLayoutBinding> layoutBindings);
                inline VkDescriptorSetLayout Get() { return _descriptorSetLayout; };

            private:
                VkDescriptorSetLayout _descriptorSetLayout;
            };
        }
    }
}
