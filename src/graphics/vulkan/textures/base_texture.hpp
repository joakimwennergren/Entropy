#pragma once

#include <spdlog/spdlog.h>

#include <ft2build.h>
#include <stb_image.h>
#include FT_FREETYPE_H

#include <graphics/vulkan/devices/ilogical_device.hpp>
#include <graphics/vulkan/devices/iphysical_device.hpp>

#include <graphics/vulkan/buffers/buffer.hpp>
#include <graphics/vulkan/commandbuffers/commandbuffer.hpp>
#include <graphics/vulkan/descriptorpools/descriptorpool.hpp>
#include <graphics/vulkan/imageviews/imageview.hpp>
#include <graphics/vulkan/memory/allocator.hpp>
#include <graphics/vulkan/synchronization/queuesync.hpp>
#include <graphics/vulkan/utilities/helpers.hpp>

#ifdef BUILD_FOR_ANDROID
#include <android/asset_manager.h>
#endif

using namespace Entropy::Graphics::Vulkan::Buffers;
using namespace Entropy::Graphics::Vulkan::CommandBuffers;
using namespace Entropy::Graphics::Vulkan::ImageViews;
using namespace Entropy::Graphics::Vulkan::Memory;
using namespace Entropy::Graphics::Vulkan::Synchronization;
using namespace Entropy::Graphics::Vulkan::DescriptorPools;

namespace Entropy::Graphics::Vulkan::Textures {
 struct BaseTexture {
  /**
   * Constructor for the BaseTexture class.
   *
   * Initializes the resources for texture management using the service locator pattern.
   * Retrieves instances of physical device, logical device, descriptor pool, and allocator from the service locator.
   *
   * @return An initialized instance of the BaseTexture class.
   */
  BaseTexture() {
   const ServiceLocator *sl = ServiceLocator::GetInstance();
   _physicalDevice = sl->getService<IPhysicalDevice>();
   _logicalDevice = sl->getService<ILogicalDevice>();
   _descriptorPool = sl->getService<IDescriptorPool>();
   _allocator = sl->getService<IAllocator>();
  }

  /**
   * Destructor for the BaseTexture class.
   *
   * Cleans up Vulkan resources associated with the texture. It destroys the Vulkan image
   * and frees the descriptor sets. Ensures that the logical device and descriptor pool
   * acquired during initialization are properly released.
   */
  ~BaseTexture() {
   vkDestroyImage(_logicalDevice->Get(),
                  _textureImage, nullptr);
   vkFreeDescriptorSets(_logicalDevice->Get(),
                        _descriptorPool->Get(), 1, &descriptorSet);
   if (textureSampler != nullptr) {
    vkDestroySampler(_logicalDevice->Get(), textureSampler, nullptr);
   }
  }


  /**
   * Transitions the layout of an image in Vulkan from one layout to another.
   *
   * This method records a command buffer with a pipeline barrier that transitions
   * the image from the specified old layout to the new layout. It handles synchronization
   * and layout transitions for optimal performance.
   *
   * @param image The Vulkan image to transition.
   * @param oldLayout The current layout of the image.
   * @param newLayout The desired new layout for the image.
   */
  void TransitionImageLayout(VkImage image, VkImageLayout oldLayout,
                             VkImageLayout newLayout) const;

  /**
   * Copies data from a buffer to an image.
   *
   * This method is used to transfer data from a specified VkBuffer to a specified VkImage.
   * It ensures that the image is in the correct transfer destination layout and performs
   * the copy operation.
   *
   * @param buffer The VkBuffer containing the data to be copied.
   *               Must not be VK_NULL_HANDLE.
   * @param image The VkImage where the data will be copied to.
   *              Must not be VK_NULL_HANDLE.
   * @param width The width of the image.
   *              Must be greater than 0.
   * @param height The height of the image.
   *               Must be greater than 0.
   */
  void CopyBufferToImage(VkBuffer buffer, VkImage image,
                         uint32_t width, uint32_t height) const;

  /**
   * Creates an image with specified parameters.
   *
   * This method initializes and creates a Vulkan image object with the provided width, height, format, tiling, and usage parameters.
   *
   * @param width The width of the image to be created.
   * @param height The height of the image to be created.
   * @param format The format of the image.
   * @param tiling The tiling arrangement of the image.
   * @param usage The intended usage of the image (e.g., storage, render target).
   * @param image A reference to a VkImage object where the created image will be stored.
   */
  void CreateImage(uint32_t width, uint32_t height,
                   VkFormat format, VkImageTiling tiling,
                   VkImageUsageFlags usage, VkImage &image);

  /**
   * Retrieves the color format used for texture sampling in the BaseTexture class.
   *
   * This method determines the appropriate Vulkan color format to use based on
   * the underlying operating system defined during the build process.
   *
   * @return A Vulkan format (VkFormat) representing the color format of the texture.
   */
  static VkFormat GetColorFormat();

  VkImage _textureImage = VK_NULL_HANDLE;
  VmaAllocation _allocation = VK_NULL_HANDLE;
  VkDescriptorSetLayout _descriptorSetLayout = VK_NULL_HANDLE;
  VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
  VkSampler textureSampler = VK_NULL_HANDLE;
  std::shared_ptr<ImageView> imageView;

  std::shared_ptr<IPhysicalDevice> _physicalDevice;
  std::shared_ptr<ILogicalDevice> _logicalDevice;
  std::shared_ptr<IDescriptorPool> _descriptorPool;
  std::shared_ptr<IAllocator> _allocator;
 };
} // namespace Entropy::Graphics::Vulkan::Textures
