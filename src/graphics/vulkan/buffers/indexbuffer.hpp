#ifndef ENTROPY_INDEXBUFFER_H
#define ENTROPY_INDEXBUFFER_H

#include <graphics/vulkan/buffers/buffer.hpp>

namespace Entropy::Graphics::Vulkan::Buffers {
    template<class T>
    /**
     * @brief Represents a line defined by its start and end points, thickness, and color.
     *
     * Constructs a textured and colored quad that visually represents the line segment
     * between the given start and end positions. The line is rendered as a rectangular
     * quad with a thickness specified in the implementation.
     *
     * Creates the vertex data for the quad using the positions, assigns texture
     * coordinates, and prepares the necessary buffers for rendering.
     *
     * @param start The starting point of the line in 3D space.
     * @param end The ending point of the line in 3D space.
     * @param color The color of the line in RGBA format.
     *
     * @details
     * - Calculates a direction vector for the line segment and derives a perpendicular vector
     *   to compute the quad's geometry.
     * - Half-thickness is used to offset each point to form the quad's vertices.
     * - A default texture is loaded and assigned to the line.
     *
     * @throws std::runtime_error if buffer creation or memory allocation fails.
     */
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
            vmaMapMemory(_allocator->Get(), allocation, &_mappedMemory);
            memcpy(_mappedMemory, indices.data(), bufferSize);
            vmaUnmapMemory(_allocator->Get(), allocation);
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
            vmaMapMemory(_allocator->Get(), allocation, &_mappedMemory);
            vmaUnmapMemory(_allocator->Get(), allocation);
        }
    };
} // namespace Entropy::Graphics::Vulkan::Buffers

#endif // ENTROPY_INDEXBUFFER_H
