#pragma once

#include <graphics/vulkan/buffers/buffer.hpp>
#include <graphics/vulkan/vulkan_backend.hpp>

using namespace Entropy::Graphics::Vulkan::Buffers;
using namespace Entropy::Graphics::Vulkan;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Buffers {

/**
 * @brief Uniform buffer
 * @author Joakim Wennergren
 * @since Wed Jul 03 2024
 */
struct UniformBuffer : public Buffer {
  /**
   * @brief Constructor for uniform buffer
   * @param backend VulkanBackend
   * @param bufferSize size of buffer
   */
  UniformBuffer(Vulkan::VulkanBackend backend, VkDeviceSize bufferSize)
      : Buffer(backend) {
    assert(bufferSize != 0);
    CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
    vmaMapMemory(_vkBackend.allocator.Get(), _allocation, &_mappedMemory);
    vmaUnmapMemory(_vkBackend.allocator.Get(), _allocation);
  }
};
} // namespace Buffers
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy
