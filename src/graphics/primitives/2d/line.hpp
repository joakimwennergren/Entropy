#pragma once

#include <data/vertex.hpp>
#include <filesystem/filesystem.hpp>
#include <graphics/vulkan/buffers/indexbuffer.hpp>
#include <graphics/vulkan/buffers/vertexbuffer.hpp>
#include <graphics/vulkan/descriptorpools/descriptorpool.hpp>
#include <graphics/vulkan/textures/texture.hpp>

using namespace Entropy::Graphics::Vulkan::Textures;
using namespace Entropy::Graphics::Vulkan::Buffers;
using namespace Entropy::Graphics::Vulkan::DescriptorPools;
using namespace Entropy::Data;

namespace Entropy {
namespace Graphics {
namespace Primitives {
class Line {
public:
  /**
   * @brief Construct a new Quad object
   *
   * @param context
   */
  Line(glm::vec3 start, glm::vec3 end, glm::vec4 color);

  VkDescriptorSet _descriptorSet = VK_NULL_HANDLE;
  VkDescriptorSetLayout _descriptorSetLayout = VK_NULL_HANDLE;

  std::unique_ptr<Texture> _blank;
  std::unique_ptr<VertexBuffer> vertexBuffer;
  std::unique_ptr<IndexBuffer<uint16_t>> indexBuffer;
  void UpdateDescriptorSets();
  /**
   * @brief Destroy the Quad object
   *
   */
  ~Line();

private:
  VkSampler _textureSampler;
};
} // namespace Primitives
} // namespace Graphics
} // namespace Entropy
