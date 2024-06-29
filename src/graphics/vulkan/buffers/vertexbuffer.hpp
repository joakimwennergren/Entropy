#pragma once

#include <graphics/vulkan/vulkan_backend.hpp>
#include <graphics/data/vertex.hpp>
#include <graphics/vulkan/buffers/buffer.hpp>

using namespace Entropy::Graphics::Vulkan::Buffers;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Buffers {
class VertexBuffer : public Buffer {
public:
  VertexBuffer(Vulkan::VulkanBackend backend, std::vector<Vertex> vertices)
      : Buffer(backend) {
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                                 VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

    // @todo ?? map the memory
    vmaMapMemory(_vkBackend.allocator.Get(), _allocation, &_mappedMemory);
    memcpy(_mappedMemory, vertices.data(), bufferSize);
    vmaUnmapMemory(_vkBackend.allocator.Get(), _allocation);
  }

private:
};
} // namespace Buffers
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy