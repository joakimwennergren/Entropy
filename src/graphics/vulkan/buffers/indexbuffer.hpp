#pragma once

#include <graphics/vulkan/buffers/buffer.hpp>
#include <graphics/vulkan/vulkan_backend.hpp>

using namespace Entropy::Graphics::Vulkan::Buffers;
using namespace Entropy::Graphics::Vulkan;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Buffers {

template <class T> struct IndexBuffer : public Buffer {
  /**
   * @brief Constructor for indexbuffer
   * @param backend VulkanBackend
   * @param indices T indices
   */
  IndexBuffer(VulkanBackend backend, std::vector<T> indices) : Buffer(backend) {

    VkDeviceSize bufferSize = sizeof(T) * indices.size();

    assert(bufferSize != 0);

    CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                                 VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

    vmaMapMemory(_vkBackend.allocator.Get(), _allocation, &_mappedMemory);
    memcpy(_mappedMemory, indices.data(), bufferSize);
    vmaUnmapMemory(_vkBackend.allocator.Get(), _allocation);
  }

  /**
   * @brief Constructor for indexbuffer
   * @param backend VulkanBackend
   * @param indices T indices
   */
  IndexBuffer(VulkanBackend backend, size_t size) : Buffer(backend) {
    assert(size != 0);

    CreateBuffer(size, VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                           VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

    vmaMapMemory(_vkBackend.allocator.Get(), _allocation, &_mappedMemory);
    // vmaUnmapMemory(_vkBackend.allocator.Get(), _allocation);
  }
};
} // namespace Buffers
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy
