#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

#include <services/service.hpp>

#include <flecs/flecs.h>

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
                std::vector<flecs::entity> _deletables;
                bool hold = false;
            };
        }
    }
}
