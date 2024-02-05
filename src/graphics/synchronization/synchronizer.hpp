#pragma once

#include <vulkan/vulkan.hpp>
#include <vector>
#include <servicelocators/servicelocator.hpp>
#include <graphics/devices/logical_device.hpp>

#include "spdlog/spdlog.h"

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
                /**
                 * @brief Construct a new Renderer object
                 *
                 * @param context
                 */
                Synchronizer(unsigned int numObjects, std::shared_ptr<ServiceLocator> serviceLocator);

                /**
                 * @brief
                 *
                 */
                ~Synchronizer();

                inline std::vector<VkSemaphore> GetImageSemaphores() { return _imageSemaphores; };
                inline std::vector<VkSemaphore> GetRenderFinishedSemaphores() { return _renderFinishedSemaphores; };
                inline std::vector<VkFence> GetFences() { return _fences; };

            private:
                unsigned int _numObjects;

                // Semaphores
                std::vector<VkSemaphore> _imageSemaphores;
                std::vector<VkSemaphore> _renderFinishedSemaphores;

                // Fences
                std::vector<VkFence> _fences;

                std::shared_ptr<LogicalDevice> _logicalDevice;
            };
        }
    }
}
