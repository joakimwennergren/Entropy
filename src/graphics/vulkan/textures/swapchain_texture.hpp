#pragma once

#include <graphics/vulkan/commandbuffers/commandbuffer.hpp>
#include <graphics/vulkan/imageviews/imageview.hpp>
#include <graphics/vulkan/memory/allocator.hpp>
#include <graphics/vulkan/textures/base_texture.hpp>
#include <graphics/vulkan/utilities/helpers.hpp>
#include <vulkan/vulkan.hpp>

using namespace Entropy::Graphics::Vulkan::CommandBuffers;
using namespace Entropy::Graphics::Vulkan::Devices;
using namespace Entropy::Graphics::Vulkan::ImageViews;
using namespace Entropy::Graphics::Vulkan::Memory;

namespace Entropy::Graphics::Vulkan::Textures {
  struct SwapChainTexture : BaseTexture {
    /**
     * Creates a SwapChainTexture object with the specified width and height.
     * Initializes Vulkan image and image view for the swapchain texture.
     *
     * @param width The width of the SwapChainTexture.
     * @param height The height of the SwapChainTexture.
     * @return No return value.
     */
    SwapChainTexture(const unsigned int width, const unsigned int height) {
      const ServiceLocator *sl = ServiceLocator::GetInstance();
      const auto allocator = sl->getService<IAllocator>();

      VkImageCreateInfo imageInfo{};
      imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
      imageInfo.imageType = VK_IMAGE_TYPE_2D;
      imageInfo.extent.width = width;
      imageInfo.extent.height = height;
      imageInfo.extent.depth = 1;
      imageInfo.mipLevels = 1;
      imageInfo.arrayLayers = 1;
      imageInfo.format = textureFormat;
      imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
      imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
      imageInfo.usage =
          VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
      imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
      imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

      VmaAllocationCreateInfo allocCreateInfo = {};
      allocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
      allocCreateInfo.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;

      VK_CHECK(vmaCreateImage(allocator->Get(), &imageInfo, &allocCreateInfo,
        &_textureImage, &_allocation, nullptr));

      imageView = std::make_shared<ImageView>(_textureImage, textureFormat);
    };

    VkFormat textureFormat = VK_FORMAT_B8G8R8A8_UNORM;

  private:
    VmaAllocation _allocation = VK_NULL_HANDLE;
  };
} // namespace Entropy::Graphics::Vulkan::Textures
