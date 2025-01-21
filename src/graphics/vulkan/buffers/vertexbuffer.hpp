#ifndef ENTROPY_VERTEXBUFFER_H
#define ENTROPY_VERTEXBUFFER_H

#include <data/vertex.hpp>
#include <graphics/vulkan/buffers/buffer.hpp>

namespace Entropy::Graphics::Vulkan::Buffers {
    /**
     * A specialized buffer for storing vertex data.
     * This class extends the Buffer base class to specifically handle vertex-related data.
     */
    struct VertexBuffer : Buffer {
        /**
         * Constructs a VertexBuffer with a given set of vertices.
         * This constructor initializes a buffer, maps its memory, and copies the vertex data to it.
         *
         * @param vertices A vector containing the vertex data to be stored in the buffer.
         */
        explicit VertexBuffer(const std::vector<Data::Vertex> &vertices) {
            const VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
            assert(bufferSize != 0);

            CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                                     VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
            vmaMapMemory(_allocator->Get(), allocation, &_mappedMemory);
            memcpy(_mappedMemory, vertices.data(), bufferSize);
            vmaUnmapMemory(_allocator->Get(), allocation);
        }

        /**
         * Constructs a VertexBuffer with a specified size.
         * This constructor initializes a buffer and maps its memory.
         *
         * @param size The size of the vertex buffer to be created.
         *             The size must be non-zero, which is enforced by an assertion.
         *
         * @return A new VertexBuffer object initialized with the specified size.
         */
        explicit VertexBuffer(const size_t size) : Buffer() {
            assert(size != 0);

            CreateBuffer(size, VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                               VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
            vmaMapMemory(_allocator->Get(), allocation, &_mappedMemory);
            vmaUnmapMemory(_allocator->Get(), allocation);
        }
    };
} // namespace Entropy::Graphics::Vulkan::Buffers

#endif // ENTROPY_VERTEXBUFFER_H
