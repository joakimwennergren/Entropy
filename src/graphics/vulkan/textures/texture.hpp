#pragma once

#include <graphics/vulkan/commandpools/commandpool.hpp>
#include <string>

#include <spdlog/spdlog.h>

#include <ft2build.h>
#include <stb_image.h>
#include FT_FREETYPE_H

#include "config.hpp"

#include <graphics/vulkan/devices/ilogical_device.hpp>
#include <graphics/vulkan/devices/iphysical_device.hpp>

#include <graphics/vulkan/buffers/buffer.hpp>
#include <graphics/vulkan/buffers/stagedbuffer.hpp>
#include <graphics/vulkan/commandbuffers/commandbuffer.hpp>
#include <graphics/vulkan/descriptorpools/descriptorpool.hpp>
#include <graphics/vulkan/imageviews/imageview.hpp>
#include <graphics/vulkan/memory/allocator.hpp>
#include <graphics/vulkan/synchronization/queuesync.hpp>
#include <graphics/vulkan/utilities/utilities.hpp>

#ifdef BUILD_FOR_ANDROID
#include <android/asset_manager.h>
#endif

using namespace Entropy::Graphics::Vulkan::Buffers;
using namespace Entropy::Graphics::Vulkan::Utilities;
using namespace Entropy::Graphics::Vulkan::CommandBuffers;
using namespace Entropy::Graphics::Vulkan::ImageViews;
using namespace Entropy::Graphics::Vulkan::Memory;
using namespace Entropy::Graphics::Vulkan::Synchronization;
using namespace Entropy::Graphics::Vulkan::DescriptorPools;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Textures {

class Texture {
public:
  Texture() {
    ServiceLocator *sl = ServiceLocator::GetInstance();
    _physicalDevice = sl->getService<IPhysicalDevice>();
    _logicalDevice = sl->getService<ILogicalDevice>();
    _descriptorPool = sl->getService<IDescriptorPool>();
    _allocator = sl->getService<IAllocator>();
  }

  ~Texture() {
    // vkDeviceWaitIdle(_vulkanBackend.logicalDevice.Get());
    // vkDestroyImageView(_vulkanBackend.logicalDevice.Get(), _imageView,
    // nullptr); vkDestroyImage(_vulkanBackend.logicalDevice.Get(),
    // _textureImage, nullptr); if(_descriptorSet != nullptr)
    // {
    //   vkFreeDescriptorSets(_vulkanBackend.logicalDevice.Get(),
    //                      _descriptorPool.Get(), 1, &_descriptorSet);
    // }
  }

  void CreateTextureImage(std::string path);

  void CreateTextureImageFromPixels(unsigned char *pixels, int width,
                                    int height);

  // void CreateTextureImageFromKtx(unsigned char *pixels, unsigned int width,
  // unsigned int height, int size, int mips, VkFormat format, ktxTexture
  // *ktxTexture);

  void CreateTextureImageFromBuffer(FT_Bitmap bitmap);

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

  VkImage _textureImage = VK_NULL_HANDLE;
  VkImageView _imageView = VK_NULL_HANDLE;
  VmaAllocation _allocation = VK_NULL_HANDLE;

  VkDescriptorSet _descriptorSet = VK_NULL_HANDLE;

  std::shared_ptr<IPhysicalDevice> _physicalDevice;
  std::shared_ptr<ILogicalDevice> _logicalDevice;
  std::shared_ptr<IDescriptorPool> _descriptorPool;
  std::shared_ptr<IAllocator> _allocator;
};
} // namespace Textures
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy
