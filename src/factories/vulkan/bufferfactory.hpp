#pragma once
#include "../../graphics/vulkan/buffers/indexbuffer.hpp"
#include "../../graphics/vulkan/buffers/stagedbuffer.hpp"
#include "../../graphics/vulkan/buffers/storagebuffer.hpp"
#include "../../graphics/vulkan/buffers/uniformbuffer.hpp"
#include "../../graphics/vulkan/buffers/vertexbuffer.hpp"
#include "../../graphics/vulkan/vulkan_backend.hpp"
#include "graphics/vulkan/buffers/storagebuffer.hpp"
#include "vulkan/vulkan_core.h"

using namespace Entropy::Graphics::Vulkan;
using namespace Entropy::Graphics::Vulkan::Buffers;

namespace Entropy {
namespace Factories {
namespace Vulkan {

struct BufferFactory {

  BufferFactory(VulkanBackend backend) : _vkBackend{backend} {}

  template <typename T>
  std::shared_ptr<IndexBuffer<T>> CreateIndexBuffer(std::vector<T> indices) {
    return std::make_shared<IndexBuffer<T>>(_vkBackend, indices);
  }

  std::shared_ptr<VertexBuffer>
  CreateVertexBuffer(std::vector<Vertex> vertices) {
    return std::make_shared<VertexBuffer>(_vkBackend, vertices);
  }

  std::shared_ptr<StagedBuffer> CreateStagingBuffer(VkDeviceSize size,
                                                    uint8_t *data,
                                                    VkBufferUsageFlags flags) {
    return std::make_shared<StagedBuffer>(_vkBackend, size, data, flags);
  }

  std::shared_ptr<StorageBuffer> CreateStorageBuffer(VkDeviceSize size,
                                                     uint8_t *data) {
    return std::make_shared<StorageBuffer>(_vkBackend, size, data);
  }

  std::shared_ptr<UniformBuffer> CreateUniformBuffer(VkDeviceSize bufferSize) {
    return std::make_shared<UniformBuffer>(_vkBackend, bufferSize);
  }

  // Vulkan Dependency
  VulkanBackend _vkBackend;
};

} // namespace Vulkan
} // namespace Factories
} // namespace Entropy