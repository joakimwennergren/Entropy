#pragma once

#include <graphics/vulkan/buffers/buffer.hpp>

using namespace Entropy::Graphics::Vulkan::Buffers;
using namespace Entropy::Graphics::Vulkan;

namespace Entropy::Graphics::Vulkan::Buffers {
  struct StagingBuffer : Buffer {
    /**
     * Constructs a StagedBuffer with the specified size, data, and usage flags.
     *
     * @param size The size of the buffer in bytes.
     * @param data A pointer to the data to be copied into the buffer. Can be nullptr.
     * @param flags The usage flags specifying how the buffer will be used.
     * @return A new StagedBuffer object.
     */
    StagingBuffer(const VkDeviceSize size, const uint8_t *data,
                  const VkBufferUsageFlags flags) {
      CreateBuffer(size, flags);

      vmaMapMemory(_allocator->Get(), _allocation, &_mappedMemory);

      if (data != nullptr) {
        memcpy(_mappedMemory, data, static_cast<size_t>(size));
      }

      vmaUnmapMemory(_allocator->Get(), _allocation);
    }
  };
} // namespace Entropy::Graphics::Vulkan::Buffers
