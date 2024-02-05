#pragma once

#include <vulkan/vulkan.hpp>
#include <iostream>
#include <set>
#include <string>
#include <config.hpp>

#include <graphics/surfaces/surface.hpp>
#include <graphics/queuefamilies/queuefamily.hpp>
#include <graphics/devices/physical_device.hpp>
#include <services/service.hpp>

using namespace Entropy::Graphics::Surfaces;
using namespace Entropy::Graphics::QueueFamilies;
using namespace Entropy::Graphics::Devices;

namespace Entropy
{
    namespace Graphics
    {
        namespace Devices
        {
            class LogicalDevice : public Service
            {
            public:
                LogicalDevice(std::shared_ptr<PhysicalDevice> physicalDevice, std::shared_ptr<WindowSurface> surface);
                inline VkDevice Get() { return _logicalDevice; };
                VkQueue GetGraphicQueue() { return _graphicsQueue; };
                VkQueue GetPresentQueue() { return _presentQueue; };

            private:
                VkDevice _logicalDevice;
                VkQueue _graphicsQueue;
                VkQueue _presentQueue;
            };
        }
    }
}
