#pragma once

#include "graphics/vulkan/utilities/utilities.hpp"
#include <graphics/vulkan/commandbuffers/commandbuffer.hpp>
#include <graphics/vulkan/imageviews/imageview.hpp>
#include <graphics/vulkan/memory/allocator.hpp>
#include <graphics/vulkan/textures/base_texture.hpp>
#include <spdlog/spdlog.h>
#include <vulkan/vulkan.hpp>

using namespace Entropy::Graphics::Vulkan::CommandBuffers;
using namespace Entropy::Graphics::Vulkan::SwapChains;
using namespace Entropy::Graphics::Vulkan::Devices;
using namespace Entropy::Graphics::Vulkan::ImageViews;
using namespace Entropy::Graphics::Vulkan::Memory;
using namespace Entropy::Graphics::Vulkan::Textures;

namespace Entropy::Graphics::Vulkan::Textures {
  struct DepthBufferTexture : BaseTexture {
    /**
     * Constructs a DepthBufferTexture with the specified width and height.
     *
     * @param width The width of the texture.
     * @param height The height of the texture.
     * @return A new DepthBufferTexture instance.
     */
    DepthBufferTexture(const uint32_t width, const uint32_t height) {
      const ServiceLocator *sl = ServiceLocator::GetInstance();
      const auto allocator = sl->getService<IAllocator>();
      _physicalDevice = sl->getService<IPhysicalDevice>();

      VkFormat depthFormat =
          findSupportedFormat({
                                VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT,
                                VK_FORMAT_D24_UNORM_S8_UINT
                              },
                              VK_IMAGE_TILING_OPTIMAL,
                              VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

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
     * Finds a supported Vulkan format from the given list of candidates that matches the required
     * image tiling and format features.
     *
     * @param candidates A vector of candidate VkFormat values to search through.
     * @param tiling The desired VkImageTiling for the format.
     * @param features The required VkFormatFeatureFlags for the format.
     * @return The VkFormat that matches the specified criteria, or VK_FORMAT_UNDEFINED if no match is found.
     */
    [[nodiscard]] VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates,
                                               VkImageTiling tiling,
                                               VkFormatFeatureFlags features) const {
      for (const VkFormat format: candidates) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(_physicalDevice->Get(), format,
                                            &props);

        if (tiling == VK_IMAGE_TILING_LINEAR &&
            (props.linearTilingFeatures & features) == features) {
          return format;
        } else if (tiling == VK_IMAGE_TILING_OPTIMAL &&
                   (props.optimalTilingFeatures & features) == features) {
          return format;
        }
      }

      spdlog::warn("Failed to find supported format.");
      return VK_FORMAT_UNDEFINED;
    }

    VmaAllocation _allocation = VK_NULL_HANDLE;
    std::shared_ptr<IPhysicalDevice> _physicalDevice;
  };
} // namespace Entropy::Graphics::Vulkan::Textures
