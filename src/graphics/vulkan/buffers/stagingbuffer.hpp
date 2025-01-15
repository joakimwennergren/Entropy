#ifndef ENTROPY_STAGINGBUFFER_H
#define ENTROPY_STAGINGBUFFER_H

#include <graphics/vulkan/buffers/buffer.hpp>

namespace Entropy::Graphics::Vulkan::Buffers {
  /**
   * Represents a staging buffer, which is a temporary buffer used for transferring data between the CPU and GPU.
   * Inherits from the Buffer class.
   */
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

      vmaMapMemory(_allocator->Get(), allocation, &_mappedMemory);

      if (data != nullptr) {
        memcpy(_mappedMemory, data, size);
      }

      vmaUnmapMemory(_allocator->Get(), allocation);
    }
  };
} // namespace Entropy::Graphics::Vulkan::Buffers

#endif // ENTROPY_STAGEBUFFER_H
