#pragma once

#include <graphics/vulkan/buffers/buffer.hpp>

using namespace Entropy::Graphics::Vulkan::Buffers;

namespace Entropy::Graphics::Vulkan::Buffers {
    template<class T>
    struct IndexBuffer : Buffer {
        /**
         * @brief Initializes an index buffer with the provided indices.
         *
         * Allocates Vulkan buffer memory and maps it for use. The buffer is created with both
         * transfer and index buffer usage flags. The indices are copied into the mapped memory.
         *
         * @param indices A vector containing the indices to be used in the index buffer.
         *
         * @pre The vector `indices` must not be empty; this is enforced with an assertion.
         *
         * @throws std::runtime_error if buffer creation or memory allocation fails.
         */
        explicit IndexBuffer(std::vector<T> indices) : Buffer() {
            const VkDeviceSize bufferSize = sizeof(T) * indices.size();
            assert(bufferSize != 0);
            CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                                     VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
            vmaMapMemory(_allocator->Get(), _allocation, &_mappedMemory);
            memcpy(_mappedMemory, indices.data(), bufferSize);
            vmaUnmapMemory(_allocator->Get(), _allocation);
        }

        /**
         * @brief Constructs an IndexBuffer with a specified size.
         *
         * Allocates Vulkan buffer memory and maps it for use. The buffer is created with both
         * transfer and index buffer usage flags.
         *
         * @param size The size of the buffer to be created in bytes.
         *
         * @pre The parameter `size` must not be zero; this is enforced with an assertion.
         *
         * @throws std::runtime_error if buffer creation or memory allocation fails.
         */
        explicit IndexBuffer(const size_t size) : Buffer() {
            assert(size != 0);
            CreateBuffer(size, VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                               VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
            vmaMapMemory(_allocator->Get(), _allocation, &_mappedMemory);
            vmaUnmapMemory(_allocator->Get(), _allocation);
        }
    };
} // namespace Entropy::Graphics::Vulkan::Buffers
