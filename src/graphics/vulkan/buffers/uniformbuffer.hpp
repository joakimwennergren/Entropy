#pragma once

#include <graphics/vulkan/buffers/buffer.hpp>

using namespace Entropy::Graphics::Vulkan::Buffers;
using namespace Entropy::Graphics::Vulkan;

namespace Entropy::Graphics::Vulkan::Buffers {
    struct UniformBuffer : Buffer {
        /**
         * Constructs a UniformBuffer with the specified buffer size.
         *
         * This constructor initializes the UniformBuffer by creating a buffer
         * with the given size and sets it for usage as a uniform buffer.
         * It also maps and then immediately unmaps the buffer memory.
         *
         * @param bufferSize Size of the uniform buffer to be created.
         */
        explicit UniformBuffer(const VkDeviceSize bufferSize)
            : Buffer() {
            assert(bufferSize != 0);
            CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
            vmaMapMemory(_allocator->Get(), _allocation, &_mappedMemory);
            vmaUnmapMemory(_allocator->Get(), _allocation);
        }
    };
} // namespace Entropy::Graphics::Vulkan::Buffers
