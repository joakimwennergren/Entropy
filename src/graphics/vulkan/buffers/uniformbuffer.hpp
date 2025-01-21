#ifndef ENTROPY_UNIFORMBUFFER_H
#define ENTROPY_UNIFORMBUFFER_H

#include <graphics/vulkan/buffers/buffer.hpp>

namespace Entropy::Graphics::Vulkan::Buffers {
    /**
     * Represents a Uniform Buffer in Vulkan.
     *
     * This class inherits from the Buffer class and provides functionality
     * specific to creating and working with uniform buffers in Vulkan. It
     * ensures the allocated memory for the buffer is properly initialized
     * and resource management is handled securely.
     */
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
            vmaMapMemory(_allocator->Get(), allocation, &_mappedMemory);
            vmaUnmapMemory(_allocator->Get(), allocation);
        }
    };
} // namespace Entropy::Graphics::Vulkan::Buffers

#endif // ENTROPY_UNIFORMBUFFER_H
