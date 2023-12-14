#pragma once

#include <vulkan/vulkan.hpp>
#include <iostream>
#include <set>
#include <string>
#include <config.hpp>

#include <graphics/surfaces/surface.hpp>
#include <graphics/queuefamilies/queuefamily.hpp>
#include <graphics/devices/logical_device.hpp>

using namespace Entropy::Graphics::Surfaces;
using namespace Entropy::Graphics::QueueFamilies;
using namespace Entropy::Graphics::Devices;

namespace Entropy
{
    namespace Graphics
    {
        namespace DescriptorPools
        {
            class DescriptorPool
            {
            public:
                DescriptorPool(std::shared_ptr<LogicalDevice> logicalDevice);

            private:
                VkDescriptorPool _descriptorPool;
            };
        }
    }
}