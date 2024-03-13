#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

#include <services/service.hpp>

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
                std::vector<VkCommandBuffer> commandBuffers;
            };
        }
    }
}
