#pragma once

#include <graphics/vulkan/buffers/buffer.hpp>

using namespace Entropy::Graphics::Vulkan::Buffers;

namespace Entropy::Graphics::Vulkan::Buffers {
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
      vmaMapMemory(_allocator->Get(), _allocation, &_mappedMemory);
      vmaUnmapMemory(_allocator->Get(), _allocation);
    }
  };
} // namespace Entropy::Graphics::Vulkan::Buffers
