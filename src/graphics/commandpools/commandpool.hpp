#pragma once

#include <vulkan/vulkan.hpp>
#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <config.hpp>

#include <graphics/devices/logical_device.hpp>
#include <graphics/devices/physical_device.hpp>
#include <graphics/surfaces/surface.hpp>
#include <graphics/queuefamilies/queuefamily.hpp>
#include <services/service.hpp>

using namespace Entropy::Graphics::Surfaces;
using namespace Entropy::Graphics::QueueFamilies;
using namespace Entropy::Graphics::Devices;
using namespace Entropy::Services;

namespace Entropy
{
    namespace Graphics
    {
        namespace CommandPools
        {
            class CommandPool : public Service
            {
            public:
                CommandPool(std::shared_ptr<LogicalDevice> logicalDevice, std::shared_ptr<PhysicalDevice> physicalDevice, std::shared_ptr<WindowSurface> surface);
                inline VkCommandPool Get() { return _commandPool; };
                inline bool isValid() override { return true; };

            private:
                VkCommandPool _commandPool;
            };
        }
    }
}