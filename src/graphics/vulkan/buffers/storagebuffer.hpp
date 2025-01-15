#ifndef ENTROPY_STORAGEBUFFER_H
#define ENTROPY_STORAGEBUFFER_H

#include <graphics/vulkan/buffers/buffer.hpp>

namespace Entropy::Graphics::Vulkan::Buffers {
  /**
   * Represents a storage buffer for Vulkan applications.
   * The StorageBuffer class extends the Buffer class and provides
   * functionality for creating and managing storage buffers.
   *
   * This class utilizes Vulkan Memory Allocator (VMA) to handle
   * the creation, mapping, and unmapping of buffer memory.
   */
  struct StorageBuffer : Buffer {
    /**
     * Constructor for StorageBuffer.
     * Creates a storage buffer, maps its memory, and then unmaps it.
     *
     * @param bufferSize The size of the buffer to allocate.
     * @return A newly created StorageBuffer object.
     */
    explicit StorageBuffer(const VkDeviceSize bufferSize)
      : Buffer() {
      CreateBuffer(bufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
      vmaMapMemory(_allocator->Get(), allocation, &_mappedMemory);
      vmaUnmapMemory(_allocator->Get(), allocation);
    }
  };
} // namespace Entropy::Graphics::Vulkan::Buffers

#endif // ENTROPY_STORAGEBUFFER_H
