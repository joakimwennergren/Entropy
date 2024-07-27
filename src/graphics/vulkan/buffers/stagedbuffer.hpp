#pragma once

#include "vulkan/vulkan_core.h"
#include <graphics/vulkan/buffers/buffer.hpp>

using namespace Entropy::Graphics::Vulkan::Buffers;
using namespace Entropy::Graphics::Vulkan;

namespace Entropy
{
  namespace Graphics
  {
    namespace Vulkan
    {
      namespace Buffers
      {

        struct StagedBuffer : public Buffer
        {

          /**
           * @brief Constructor for staged buffer
           * @param backend VulkanBackend
           * @param size size of the buffer
           * @param data Data to be put in the buffer
           */
          StagedBuffer(VkDeviceSize size, uint8_t *data,
                       VkBufferUsageFlags flags)
          {

            // Create the buffer
            CreateBuffer(size,
                         /*VK_BUFFER_USAGE_TRANSFER_SRC_BIT*/ flags);

            // Map the memory
            // vmaMapMemory(_vkBackend.allocator.Get(), _allocation, &_mappedMemory);

            // Copy the data
            if (data != nullptr)
            {
              memcpy(_mappedMemory, data, static_cast<size_t>(size));
            }

            // Unmap memory
            // vmaUnmapMemory(_vkBackend.allocator.Get(), _allocation);
          }
        };

      } // namespace Buffers
    } // namespace Vulkan
  } // namespace Graphics
} // namespace Entropy