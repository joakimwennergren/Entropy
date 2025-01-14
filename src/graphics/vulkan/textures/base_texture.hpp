#ifndef ENTROPY_BASE_TEXTURE_H
#define ENTROPY_BASE_TEXTURE_H

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
#include <graphics/vulkan/utilities/helpers.hpp>

#ifdef BUILD_FOR_ANDROID
#include <android/asset_manager.h>
#endif

using namespace Entropy::Graphics::Vulkan::Buffers;
using namespace Entropy::Graphics::Vulkan::CommandBuffers;
using namespace Entropy::Graphics::Vulkan::ImageViews;

namespace Entropy::Graphics::Vulkan::Textures {
 /**
  * Represents a base class for managing Vulkan textures.
  *
  * The BaseTexture class provides methods and members for creating,
  * managing, and cleaning up Vulkan textures. It integrates with Vulkan
  * services like physical devices, logical devices, descriptor pools,
  * and memory allocators through a service locator mechanism.
  */
 struct BaseTexture {
  /**
   * Constructor for the BaseTexture class.
   *
   * Initializes the BaseTexture instance by retrieving and storing essential Vulkan-related
   * services from the ServiceLocator. These services include the physical device, logical
   * device, descriptor pool, and memory allocator, which are required for managing
   * GPU resources and allocation.
   */
  BaseTexture() {
   const ServiceLocator *sl = ServiceLocator::GetInstance();
   _physicalDevice = sl->getService<IPhysicalDevice>();
   _logicalDevice = sl->getService<ILogicalDevice>();
   _descriptorPool = sl->getService<IDescriptorPool>();
   _allocator = sl->getService<IAllocator>();
  }

  /**
   * Destructor for managing the cleanup of Vulkan texture resources.
   *
   * The destructor ensures all allocated Vulkan resources associated with
   * the texture are properly destroyed, including the texture image,
   * descriptor sets, and texture sampler. It utilizes the logical device
   * and descriptor pool resources obtained from the service locator.
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

#endif // ENTROPY_BASE_TEXTURE_H
