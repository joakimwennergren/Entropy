#pragma once
#include "../../graphics/vulkan/vulkan_backend.hpp"
#include "../../graphics/vulkan/buffers/indexbuffer.hpp"
#include "../../graphics/vulkan/buffers/vertexbuffer.hpp"
#include "../../graphics/vulkan/buffers/stagedbuffer.hpp"
#include "../../graphics/vulkan/buffers/uniformbuffer.hpp"

using namespace Entropy::Graphics::Vulkan;
using namespace Entropy::Graphics::Vulkan::Buffers;


namespace Entropy {
namespace Factories {
namespace Vulkan {


struct BufferFactory {

  BufferFactory(VulkanBackend backend): _vkBackend{backend} {
  }

  template <typename T>
  IndexBuffer<T> CreateIndexBuffer(std::vector<T> indices)
  {
    return IndexBuffer<T>(_vkBackend, indices);
  }

  VertexBuffer CreateVertexBuffer(std::vector<Vertex> vertices)
  {
    return VertexBuffer(_vkBackend, vertices);
  }

  StagedBuffer CreateStagingBuffer(VkDeviceSize size, uint8_t *data)
  {
    return StagedBuffer(_vkBackend, size, data);
  }

  UniformBuffer CreateUniformBuffer(VkDeviceSize bufferSize)
  {
    return UniformBuffer(_vkBackend, bufferSize);
  }

  // Vulkan Dependency
  VulkanBackend _vkBackend;
};

} // namespace Drawables
} // namespace Vulkan
} // namespace Graphics