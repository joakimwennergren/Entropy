#pragma once

#include <graphics/vulkan/devices/logical_device.hpp>
#include <graphics/vulkan/commandbuffers/commandbuffer.hpp>
#include <graphics/vulkan/imageviews/imageview.hpp>
#include <graphics/vulkan/memory/allocator.hpp>
#include <factories/vulkan/texturefactory.hpp>
#include <graphics/vulkan/swapchains/swapchain.hpp>
#include <vulkan/vulkan.hpp>
#include <graphics/vulkan/imageviews/imageview.hpp>
#include "spdlog/spdlog.h"

using namespace Entropy::Graphics::Vulkan::CommandBuffers;
using namespace Entropy::Graphics::Vulkan::Swapchains;
using namespace Entropy::Graphics::Vulkan::Devices;
using namespace Entropy::Graphics::Vulkan::ImageViews;
using namespace Entropy::Graphics::Vulkan::Memory;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Textures {

struct DepthBufferTexture {
  DepthBufferTexture(VulkanBackend backend, QueueSync qs, Allocator a, unsigned int width, unsigned int height)
  {
    VkFormat depthFormat = FindDepthFormat(backend);

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

    VkResult res = vmaCreateImage(a.Get(), &imageInfo, &allocCreateInfo,
        &_depthImage, &_allocation, nullptr);

    spdlog::warn(res);

    _depthImageView = ImageView(backend, _depthImage, depthFormat,VK_IMAGE_ASPECT_DEPTH_BIT)
                      .Get();

  };
    
    VkImage _depthImage = VK_NULL_HANDLE;
    VkImageView _depthImageView = VK_NULL_HANDLE;
  private:

    VkFormat findSupportedFormat(VulkanBackend backend, const std::vector<VkFormat> &candidates,
                                    VkImageTiling tiling,
                                    VkFormatFeatureFlags features) {

    for (VkFormat format : candidates) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(backend.physicalDevice.Get(), format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR &&
            (props.linearTilingFeatures & features) == features) {
        return format;
        } else if (tiling == VK_IMAGE_TILING_OPTIMAL &&
                (props.optimalTilingFeatures & features) == features) {
        return format;
        }
    }

    spdlog::warn("Failed to find supported format.");
    }

    VkFormat FindDepthFormat(VulkanBackend backend) {

    return findSupportedFormat(
        backend,
        {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT,
        VK_FORMAT_D24_UNORM_S8_UINT},
        VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
    }



    VmaAllocation _allocation = VK_NULL_HANDLE;
};

} // namespace RenderPasses
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy
