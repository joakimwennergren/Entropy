#pragma once

#include <filesystem/filesystem.hpp>
#include <graphics/data/vertex.hpp>
#include <graphics/vulkan/descriptorpools/descriptorpool.hpp>
#include <graphics/vulkan/buffers/indexbuffer.hpp>
#include <graphics/vulkan/buffers/vertexbuffer.hpp>
#include <graphics/vulkan/textures/texture.hpp>

using namespace Entropy::Graphics::Vulkan::Textures;
using namespace Entropy::Graphics::Vulkan::Buffers;
using namespace Entropy::Graphics::Vulkan::DescriptorPools;

namespace Entropy {
namespace Graphics {
namespace Primitives {
class Quad {
public:
  /**
   * @brief Construct a new Quad object
   *
   * @param context
   */
  Quad(std::shared_ptr<ServiceLocator> serviceLocator);
  ~Quad();

  VkDescriptorSet _descriptorSet = VK_NULL_HANDLE;
  VkDescriptorSetLayout _descriptorSetLayout = VK_NULL_HANDLE;

  std::unique_ptr<Texture> _blank;

  std::unique_ptr<VertexBuffer> vertexBuffer;
  std::unique_ptr<IndexBuffer<uint16_t>> indexBuffer;

  void UpdateDescriptorSets();

private:
  VkSampler _textureSampler;
  std::shared_ptr<ServiceLocator> _serviceLocator;
};
} // namespace Primitives
} // namespace Graphics
} // namespace Entropy
