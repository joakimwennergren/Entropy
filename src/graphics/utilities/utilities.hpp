#pragma once

#include <vulkan/vulkan.hpp>

#include <servicelocators/servicelocator.hpp>
#include <graphics/devices/logical_device.hpp>

using namespace Entropy::ServiceLocators;
using namespace Entropy::Graphics::Devices;

namespace Entropy
{
    namespace Graphics
    {
        namespace Utilities
        {
            class Utility
            {
            public:
                static uint32_t FindMemoryTypeIndex(std::shared_ptr<ServiceLocator> serviceLocator, uint32_t typeFilter, VkMemoryPropertyFlags properties);
                static void *AlignedAlloc(size_t size, size_t alignment);
                void AlignedFree(void *data);
            };
        }
    }
}
