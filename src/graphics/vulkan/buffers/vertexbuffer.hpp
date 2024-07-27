#pragma once

#include <data/vertex.hpp>
#include <graphics/vulkan/buffers/buffer.hpp>

using namespace Entropy::Graphics::Vulkan::Buffers;
using namespace Entropy::Graphics::Vulkan;
using namespace Entropy::Data;

namespace Entropy
{
  namespace Graphics
  {
    namespace Vulkan
    {
      namespace Buffers
      {

        /**
         * @brief Vertex buffer
         * @author Joakim Wennergren
         * @since Wed Jul 03 2024
         */
        struct VertexBuffer : public Buffer
        {
          /**
           * @brief Constructor for Vertex buffer
           * @param backend VulkanBackend
           * @param vertices Vertices to be put into buffer
           */
          VertexBuffer(std::vector<Vertex> vertices)
          {

            VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

            assert(bufferSize != 0);

            CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                                         VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
            // vmaMapMemory(_vkBackend.allocator.Get(), _allocation, &_mappedMemory);
            memcpy(_mappedMemory, vertices.data(), bufferSize);
            // vmaUnmapMemory(_vkBackend.allocator.Get(), _allocation);
          }

          /**
           * @brief Constructor for Vertex buffer
           * @param backend VulkanBackend
           * @param vertices Vertices to be put into buffer
           */
          VertexBuffer(size_t size) : Buffer()
          {

            assert(size != 0);

            CreateBuffer(size, VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                                   VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
            // vmaMapMemory(_vkBackend.allocator.Get(), _allocation, &_mappedMemory);
            // vmaUnmapMemory(_vkBackend.allocator.Get(), _allocation);
          }
        };

      } // namespace Buffers
    } // namespace Vulkan
  } // namespace Graphics
} // namespace Entropy