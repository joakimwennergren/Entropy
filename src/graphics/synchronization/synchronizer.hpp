#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>
#include <spdlog/spdlog.h>

#include <servicelocators/servicelocator.hpp>
#include <graphics/devices/logical_device.hpp>

using namespace Entropy::ServiceLocators;
using namespace Entropy::Graphics::Devices;

namespace Entropy
{
    namespace Graphics
    {
        namespace Synchronization
        {
            class Synchronizer
            {
            public:
                Synchronizer(unsigned int numObjects, std::shared_ptr<ServiceLocator> serviceLocator);
                ~Synchronizer();

                inline std::vector<VkSemaphore> GetImageSemaphores() { return _imageSemaphores; };
                inline std::vector<VkSemaphore> GetRenderFinishedSemaphores() { return _renderFinishedSemaphores; };
                inline std::vector<VkFence> GetFences() { return _fences; };

            private:
                std::vector<VkSemaphore> _imageSemaphores;
                std::vector<VkSemaphore> _renderFinishedSemaphores;
                std::vector<VkFence> _fences;
                std::shared_ptr<LogicalDevice> _logicalDevice;
                unsigned int _numObjects;
            };
        }
    }
}
