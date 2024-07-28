#pragma once

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

        /**
         * @brief Uniform buffer
         * @author Joakim Wennergren
         * @since Wed Jul 03 2024
         */
        struct UniformBuffer : public Buffer
        {
          /**
           * @brief Constructor for uniform buffer
           * @param backend VulkanBackend
           * @param bufferSize size of buffer
           */
          UniformBuffer(VkDeviceSize bufferSize)
              : Buffer()
          {
            assert(bufferSize != 0);
            CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
            vmaMapMemory(_allocator->Get(), _allocation, &_mappedMemory);
            vmaUnmapMemory(_allocator->Get(), _allocation);
          }
        };
      } // namespace Buffers
    } // namespace Vulkan
  } // namespace Graphics
} // namespace Entropy
