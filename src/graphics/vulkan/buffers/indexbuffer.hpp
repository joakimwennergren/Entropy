#pragma once

#include <graphics/vulkan/buffers/buffer.hpp>
#include <graphics/vulkan/vulkan_backend.hpp>

using namespace Entropy::Graphics::Vulkan::Buffers;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Buffers {

template <class T> class IndexBuffer : public Buffer {
public:
  IndexBuffer(Vulkan::VulkanBackend backend, std::vector<T> indices)
      : Buffer(backend) {

    VkDeviceSize bufferSize = sizeof(T) * indices.size();

    assert(bufferSize != 0);

    CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                                 VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

    void *mappedData;
    vmaMapMemory(_vkBackend.allocator.Get(), _allocation, &mappedData);
    memcpy(mappedData, indices.data(), bufferSize);
    vmaUnmapMemory(_vkBackend.allocator.Get(), _allocation);
  }
};
} // namespace Buffers
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy
