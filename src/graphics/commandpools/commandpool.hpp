#pragma once

#include <vulkan/vulkan.hpp>
#include <spdlog/spdlog.h>

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

            private:
                VkCommandPool _commandPool = VK_NULL_HANDLE;
            };
        }
    }
}