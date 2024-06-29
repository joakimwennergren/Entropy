#pragma once

#include <filesystem/filesystem.hpp>
#include <graphics/data/vertex.hpp>
#include <graphics/descriptorpools/descriptorpool.hpp>
#include <graphics/vulkan/buffers/indexbuffer.hpp>
#include <graphics/vulkan/buffers/vertexbuffer.hpp>
#include <graphics/vulkan/textures/texture.hpp>

using namespace Entropy::Graphics::Vulkan::Textures;
using namespace Entropy::Graphics::DescriptorPools;

namespace Entropy {
namespace Graphics {
namespace Primitives {
class Plane {
public:
  /**
   * @brief Construct a new Plane object
   *
   * @param context
   */
  Plane(std::shared_ptr<ServiceLocator> serviceLocator);

  void Test(){};

  VkDescriptorSet _descriptorSet = VK_NULL_HANDLE;
  VkDescriptorSetLayout _descriptorSetLayout = VK_NULL_HANDLE;

  std::unique_ptr<Texture> _blank;

  void UpdateDescriptorSets();
  /**
   * @brief Destroy the Plane object
   *
   */
  ~Plane();
  std::unique_ptr<VertexBuffer> vertexBuffer;
  std::unique_ptr<IndexBuffer<uint16_t>> indexBuffer;

private:
  VkSampler _textureSampler;
  std::shared_ptr<ServiceLocator> _serviceLocator;
};
} // namespace Primitives
} // namespace Graphics
} // namespace Entropy
