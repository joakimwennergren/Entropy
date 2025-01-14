#ifndef ENTROPY_SWAPCHAIN_TEXTURE_H
#define ENTROPY_SWAPCHAIN_TEXTURE_H

#include <graphics/vulkan/imageviews/imageview.hpp>
#include <graphics/vulkan/memory/allocator.hpp>
#include <graphics/vulkan/textures/base_texture.hpp>
#include <graphics/vulkan/utilities/helpers.hpp>
#include <vulkan/vulkan.hpp>

namespace Entropy::Graphics::Vulkan::Textures {
  /**
   * @brief Represents a texture used in the swap chain for rendering. This class manages
   *        the creation and allocation of a Vulkan image and an associated image view.
   *
   * SwapChainTexture extends the functionality of BaseTexture and is designed specifically
   * for handling swap chain textures with Vulkan.
   */
  struct SwapChainTexture : BaseTexture {
    /**
     * @brief Constructs a SwapChainTexture object with the specified width and height. This initializes
     *        a Vulkan image along with its respective memory allocation and creates an associated image view.
     *
     * @param width The width of the swap chain texture in pixels.
     * @param height The height of the swap chain texture in pixels.
     */
    SwapChainTexture(const unsigned int width, const unsigned int height) {
      const auto allocator = ServiceLocator::GetInstance()->getService<IAllocator>();

      VkImageCreateInfo imageInfo{};
      imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
      imageInfo.imageType = VK_IMAGE_TYPE_2D;
      imageInfo.extent.width = width;
      imageInfo.extent.height = height;
      imageInfo.extent.depth = 1;
      imageInfo.mipLevels = 1;
      imageInfo.arrayLayers = 1;
      imageInfo.format = GetColorFormat();
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

      imageView = std::make_shared<ImageView>(_textureImage, GetColorFormat());
    };

  private:
    VmaAllocation _allocation = VK_NULL_HANDLE;
  };
} // namespace Entropy::Graphics::Vulkan::Textures

#endif // ENTROPY_SWAPCHAIN_TEXTURE_H
