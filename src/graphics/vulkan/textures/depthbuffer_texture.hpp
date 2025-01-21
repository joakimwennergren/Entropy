#ifndef ENTROPY_DEPTHBUFFER_TEXTURE_H
#define ENTROPY_DEPTHBUFFER_TEXTURE_H

#include <graphics/vulkan/utilities/helpers.hpp>
#include <graphics/vulkan/imageviews/imageview.hpp>
#include <graphics/vulkan/memory/allocator.hpp>
#include <graphics/vulkan/textures/base_texture.hpp>
#include <spdlog/spdlog.h>
#include <vulkan/vulkan.hpp>

namespace Entropy::Graphics::Vulkan::Textures {
  /**
   * Represents a depth buffer texture that utilizes Vulkan for rendering depth information.
   * This texture is optimized for use as a depth-stencil attachment in Vulkan pipelines
   * and is allocated using Vulkan Memory Allocator (VMA).
   */
  struct DepthBufferTexture : BaseTexture {
    /**
     * Constructs a DepthBufferTexture instance, initializing a Vulkan-compatible depth
     * buffer texture with the specified width and height. The texture is configured
     * for use as a depth-stencil attachment and allocated using Vulkan Memory Allocator (VMA).
     *
     * @param width The width of the depth buffer texture in pixels.
     * @param height The height of the depth buffer texture in pixels.
     * @return None
     */
    DepthBufferTexture(const uint32_t width, const uint32_t height) {
      const ServiceLocator *sl = ServiceLocator::GetInstance();
      const auto allocator = sl->getService<Memory::IAllocator>();
      _physicalDevice = sl->getService<IPhysicalDevice>();

      VkFormat depthFormat =
          FindSupportedFormat({
            VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT,
            VK_FORMAT_D24_UNORM_S8_UINT
          });

      VmaAllocationCreateInfo allocCreateInfo = {};
      allocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
      allocCreateInfo.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;

      VkImageCreateInfo imageInfo{};
      imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
      imageInfo.imageType = VK_IMAGE_TYPE_2D;
      imageInfo.extent.width = width;
      imageInfo.extent.height = height;
      imageInfo.extent.depth = 1;
      imageInfo.mipLevels = 1;
      imageInfo.arrayLayers = 1;
      imageInfo.format = depthFormat;
      imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
      imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
      imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
      imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
      imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

      VK_CHECK(vmaCreateImage(allocator->Get(), &imageInfo, &allocCreateInfo,
        &_textureImage, &_allocation, nullptr));

      imageView = std::make_shared<ImageView>(_textureImage, depthFormat,
                                              VK_IMAGE_ASPECT_DEPTH_BIT);
    };

  private:
    /**
     * Finds and returns a supported Vulkan format from a list of candidate formats.
     * The method checks for formats that have the depth-stencil attachment feature enabled.
     *
     * @param candidates A vector of Vulkan format candidates to be checked for support.
     * @return The first supported Vulkan format that meets the requirements.
     *         If no supported format is found, returns VK_FORMAT_UNDEFINED.
     */
    [[nodiscard]] VkFormat FindSupportedFormat(const std::vector<VkFormat> &candidates) const {
      for (const VkFormat format: candidates) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(_physicalDevice->Get(), format,
                                            &props);
        if ((props.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) ==
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
          return format;
        }
      }

      spdlog::warn("Failed to find supported depth buffer format.");
      return VK_FORMAT_UNDEFINED;
    }

    VmaAllocation _allocation = VK_NULL_HANDLE;
    std::shared_ptr<IPhysicalDevice> _physicalDevice;
  };
} // namespace Entropy::Graphics::Vulkan::Textures

#endif // ENTROPY_DEPTHBUFFER_TEXTURE_H
