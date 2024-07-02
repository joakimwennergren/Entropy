#pragma once

#include <graphics/vulkan/buffers/buffer.hpp>
#include <graphics/vulkan/vulkan_backend.hpp>

using namespace Entropy::Graphics::Vulkan::Buffers;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Buffers {

struct StagedBuffer : public Buffer {

  /**
   * @brief Constructor for staged buffer
   * @param backend VulkanBackend
   * @param size size of the buffer
   * @param data Data to be put in the buffer
   */
  StagedBuffer(Vulkan::VulkanBackend backend, VkDeviceSize size, uint8_t *data)
      : Buffer(backend) {

    // Create the buffer
    CreateBuffer(
        size,
        /*VK_BUFFER_USAGE_TRANSFER_SRC_BIT*/ VK_BUFFER_USAGE_TRANSFER_DST_BIT);

    // Map the memory
    vmaMapMemory(_vkBackend.allocator.Get(), _allocation, &_mappedMemory);

    // Copy the data
    if (data != nullptr) {
      memcpy(_mappedMemory, data, static_cast<size_t>(size));
    }

    // Unmap memory
    vmaUnmapMemory(_vkBackend.allocator.Get(), _allocation);
  }
};

} // namespace Buffers
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy