#pragma once

#include <graphics/vulkan/buffers/buffer.hpp>
#include <graphics/vulkan/vulkan_backend.hpp>

using namespace Entropy::Graphics::Vulkan::Buffers;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Buffers {
struct StorageBuffer : public Buffer {
  StorageBuffer(Vulkan::VulkanBackend backend, VkDeviceSize bufferSize,
                void *input)
      : Buffer(backend) {

    CreateBuffer(bufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);

    // vkMapMemory(logicalDevice->Get(), _bufferMemory, 0, bufferSize, 0,
    // &_mappedMemory);
  }

private:
};
} // namespace Buffers
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy
