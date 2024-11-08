#pragma once

#include <graphics/vulkan/buffers/buffer.hpp>

using namespace Entropy::Graphics::Vulkan::Buffers;


namespace Entropy::Graphics::Vulkan::Buffers {
    template<class T>
    struct IndexBuffer : public Buffer {
        /**
         * @class IndexBuffer
         * @brief A class that manages index buffer creation and memory handling for
         * Vulkan.
         *
         * This constructor initializes an index buffer using a vector of indices. It
         * allocates the necessary Vulkan buffer memory, maps the memory, and copies
         * the index data into it. The buffer is created with both transfer and index
         * buffer usage flags.
         *
         * @param T The data type of the indices (e.g., uint16_t or uint32_t).
         * @param indices A vector containing the index data to be stored in the
         * buffer. The size of the buffer will be calculated based on the number of
         * elements in this vector.
         *
         * @pre The vector `indices` must not be empty, as an empty buffer will
         * assert.
         *
         * @note This constructor automatically maps and unmaps the Vulkan memory for
         * the buffer, handling data transfer from host to device.
         *
         * @throws std::runtime_error if buffer creation or memory allocation fails.
         */
        IndexBuffer(std::vector<T> indices) : Buffer() {
            VkDeviceSize bufferSize = sizeof(T) * indices.size();
            assert(bufferSize != 0);
            CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                                     VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
            vmaMapMemory(_allocator->Get(), _allocation, &_mappedMemory);
            memcpy(_mappedMemory, indices.data(), bufferSize);
        }

        /**
         * @class IndexBuffer
         * @brief A class that handles the creation and management of an index buffer
         * in Vulkan.
         *
         * This constructor initializes an index buffer of a specified size. It
         * allocates the necessary Vulkan buffer memory and maps it for potential use.
         * The buffer is created with both transfer and index buffer usage flags.
         *
         * @param size The size (in bytes) of the index buffer to be created. Must be
         * greater than zero.
         *
         * @pre The parameter `size` must not be zero; this is enforced with an
         * assertion.
         *
         * @note This constructor only allocates the buffer and maps the Vulkan
         * memory. The user is responsible for filling the buffer with the appropriate
         * data after allocation.
         *
         * @throws std::runtime_error if buffer creation or memory allocation fails.
         */
        IndexBuffer(size_t size) : Buffer() {
            assert(size != 0);
            CreateBuffer(size, VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                               VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
            vmaMapMemory(_allocator->Get(), _allocation, &_mappedMemory);
        }
    };
} // namespace Entropy::Graphics::Vulkan::Buffers



