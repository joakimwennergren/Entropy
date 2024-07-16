#pragma once

#include "spdlog/spdlog.h"
#include <factories/vulkan/texturefactory.hpp>
#include <graphics/vulkan/commandbuffers/commandbuffer.hpp>
#include <graphics/vulkan/devices/logical_device.hpp>
#include <graphics/vulkan/imageviews/imageview.hpp>
#include <graphics/vulkan/memory/allocator.hpp>
#include <graphics/vulkan/swapchains/swapchain.hpp>
#include <graphics/vulkan/textures/texture.hpp>
#include <graphics/vulkan/utilities/utilities.hpp>
#include <vulkan/vulkan.hpp>

using namespace Entropy::Graphics::Vulkan::CommandBuffers;
using namespace Entropy::Graphics::Vulkan::Swapchains;
using namespace Entropy::Graphics::Vulkan::Devices;
using namespace Entropy::Graphics::Vulkan::ImageViews;
using namespace Entropy::Graphics::Vulkan::Memory;
using namespace Entropy::Graphics::Vulkan::Utilities;
namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Textures {

struct SwapChainTexture : public Texture {
  SwapChainTexture(VulkanBackend vbe, QueueSync qs, Allocator allocator,
                   Factories::Vulkan::BufferFactory bf, CommandPool cp,
                   DescriptorPool dp, unsigned int width, unsigned int height)
      : Texture(vbe, qs, allocator, bf, cp, dp) {
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

    VkResult res = vmaCreateImage(allocator.Get(), &imageInfo, &allocCreateInfo,
                                  &_textureImage, &_allocation, nullptr);

    _imageView = ImageView(vbe, _textureImage, textureFormat).Get();
  };

  VkFormat textureFormat = VK_FORMAT_B8G8R8A8_UNORM;

private:
  VmaAllocation _allocation = VK_NULL_HANDLE;
};

} // namespace Textures
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy
