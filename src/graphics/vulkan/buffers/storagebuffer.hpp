#pragma once

#include <graphics/vulkan/buffers/buffer.hpp>

using namespace Entropy::Graphics::Vulkan::Buffers;

namespace Entropy
{
  namespace Graphics
  {
    namespace Vulkan
    {
      namespace Buffers
      {
        struct StorageBuffer : public Buffer
        {
          StorageBuffer(VkDeviceSize bufferSize,
                        void *input)
              : Buffer()
          {

            CreateBuffer(bufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);

            vmaMapMemory(_allocator->Get(), _allocation, &_mappedMemory);
            vmaUnmapMemory(_allocator->Get(), _allocation);
          }

        private:
        };
      } // namespace Buffers
    } // namespace Vulkan
  } // namespace Graphics
} // namespace Entropy
