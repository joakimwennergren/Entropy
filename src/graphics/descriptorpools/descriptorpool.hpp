#pragma once

#include <vulkan/vulkan.hpp>
#include <iostream>
#include <set>
#include <string>
#include <config.hpp>

#include <graphics/surfaces/surface.hpp>
#include <graphics/queuefamilies/queuefamily.hpp>
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
        namespace DescriptorPools
        {
            class DescriptorPool : public Service
            {
            public:
                DescriptorPool(std::shared_ptr<LogicalDevice> logicalDevice);
                inline VkDescriptorPool Get() { return _descriptorPool; };
                inline bool isValid() override { return true; };

            private:
                VkDescriptorPool _descriptorPool;
            };
        }
    }
}