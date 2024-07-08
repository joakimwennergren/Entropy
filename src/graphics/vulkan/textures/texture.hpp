#pragma once

#include "factories/vulkan/bufferfactory.hpp"
#include "graphics/vulkan/commandpools/commandpool.hpp"
#include "graphics/vulkan/vulkan_backend.hpp"
#include <string>

#include <spdlog/spdlog.h>

#include <ft2build.h>
#include <stb_image.h>
#include FT_FREETYPE_H

#include "config.hpp"
#include "tiny_gltf.h"

#include <assets/textureid.hpp>
#include <graphics/vulkan/buffers/buffer.hpp>
#include <graphics/vulkan/buffers/stagedbuffer.hpp>
#include <graphics/vulkan/commandbuffers/commandbuffer.hpp>
#include <graphics/vulkan/imageviews/imageview.hpp>
#include <graphics/vulkan/memory/allocator.hpp>
#include <graphics/vulkan/synchronization/queuesync.hpp>
#include <graphics/vulkan/utilities/utilities.hpp>

// #include <ktx.h>
// #include <ktxvulkan.h>

#ifdef BUILD_FOR_ANDROID
#include <android/asset_manager.h>
#endif

using namespace Entropy::Graphics::Vulkan::Buffers;
using namespace Entropy::Graphics::Vulkan::Utilities;
using namespace Entropy::Graphics::Vulkan::CommandBuffers;
using namespace Entropy::Graphics::Vulkan::ImageViews;
using namespace Entropy::Graphics::Vulkan::Memory;
using namespace Entropy::Graphics::Vulkan::Synchronization;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Textures {

class Texture {
public:
  Texture(VulkanBackend vbe, QueueSync qs, Allocator allocator,
          Factories::Vulkan::BufferFactory bf, CommandPool cp)
      : _vulkanBackend{vbe}, _queueSync{qs}, _allocator{allocator},
        _bufferFactory{bf}, _commandPool{cp} {
    textureId = TextureId().GetId();
  }

  ~Texture() {
    spdlog::error("Deleting texture!");
    vkDestroyImageView(_vulkanBackend.logicalDevice.Get(), _imageView, nullptr);
    vkDestroyImage(_vulkanBackend.logicalDevice.Get(), _textureImage, nullptr);
  }

  void CreateTextureImage(std::string path);

  void CreateTextureImageFromPixels(unsigned char *pixels, int width,
                                    int height);

  // void CreateTextureImageFromKtx(unsigned char *pixels, unsigned int width,
  // unsigned int height, int size, int mips, VkFormat format, ktxTexture
  // *ktxTexture);

  void CreateTextureImageFromBuffer(FT_Bitmap bitmap);

  void CreateTextureFromGLTFImage(tinygltf::Image &gltfimage);

#ifdef BUILD_FOR_ANDROID
  void CreateTextureImage(std::string path, AAssetManager *assetManager);
#endif

  inline VkImageView GetImageView() { return this->_imageView; };

  void TransitionImageLayout(VkImage image, VkImageLayout oldLayout,
                             VkImageLayout newLayout);
  void CopyBufferToImage(const VkBuffer buffer, const VkImage image,
                         const uint32_t width, const uint32_t height);
  void CreateImage(const uint32_t width, const uint32_t height,
                   const VkFormat format, const VkImageTiling tiling,
                   const VkImageUsageFlags usage, VkImage &image);

  VkFormat GetColorFormat();

  unsigned int textureId;

  VkImage _textureImage = VK_NULL_HANDLE;
  VkImageView _imageView = VK_NULL_HANDLE;
  VmaAllocation _allocation = VK_NULL_HANDLE;

  VulkanBackend _vulkanBackend;
  QueueSync _queueSync;
  Allocator _allocator;
  Factories::Vulkan::BufferFactory _bufferFactory;
  CommandPool _commandPool;
};
} // namespace Textures
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy