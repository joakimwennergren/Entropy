#pragma once

#ifdef BUILD_FOR_ANDROID
#include <android/asset_manager.h>
#endif

#include <filesystem/filesystem.hpp>
#include <graphics/data/vertex.hpp>
#include <graphics/descriptorpools/descriptorpool.hpp>
#include <graphics/vulkan/buffers/indexbuffer.hpp>
#include <graphics/vulkan/buffers/vertexbuffer.hpp>
#include <graphics/vulkan/textures/texture.hpp>
#include <scripting/script.hpp>

using namespace Entropy::Graphics::Vulkan::Textures;
using namespace Entropy::Graphics::Vulkan::Buffers;
using namespace Entropy::Scripting;
using namespace Entropy::Graphics::DescriptorPools;

namespace Entropy {
namespace Graphics {
namespace Primitives {
class Sprite {
public:
  Sprite();
  ~Sprite();

  Sprite(std::shared_ptr<ServiceLocator> serviceLocator, FT_Bitmap bitmap);

  Sprite(std::shared_ptr<ServiceLocator> serviceLocator, std::string path);
#ifdef BUILD_FOR_ANDROID
  Sprite(std::shared_ptr<ServiceLocator> serviceLocator, std::string path,
         AAssetManager *assetmanager);
#endif
  Sprite(unsigned char *pixels, int width, int height);

  VkDescriptorSet _descriptorSet = VK_NULL_HANDLE;
  VkDescriptorSetLayout _descriptorSetLayout = VK_NULL_HANDLE;
  std::unique_ptr<VertexBuffer> vertexBuffer;
  std::unique_ptr<IndexBuffer<uint16_t>> indexBuffer;

private:
  VkSampler _textureSampler;
  std::shared_ptr<ServiceLocator> _serviceLocator;
  void UpdateDescriptorSets();
};
} // namespace Primitives
} // namespace Graphics
} // namespace Entropy
