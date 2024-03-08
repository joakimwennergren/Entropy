#pragma once
#include <queue>
#include <vulkan/vulkan.hpp>
#include <vector>
#include <servicelocators/servicelocator.hpp>
#include <graphics/devices/logical_device.hpp>
#include <services/service.hpp>
#include "spdlog/spdlog.h"
#include "threadsafequeue.hpp"

using namespace Entropy::ServiceLocators;
using namespace Entropy::Graphics::Devices;
using namespace Entropy::Services;

namespace Entropy
{
    namespace Graphics
    {
        namespace Synchronization
        {
            class QueueSync : public Service
            {
            public:
                /**
                 * @brief Construct a new Renderer object
                 *
                 * @param context
                 */
                QueueSync(std::shared_ptr<ServiceLocator> serviceLocator)
                {
                }

                std::vector<VkCommandBuffer> _commandBuffers;

            private:
            };
        }
    }
}
