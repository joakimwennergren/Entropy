#include "imageview.hpp"

using namespace Entropy::Graphics::Vulkan::ImageViews;
using namespace Entropy::Graphics::Vulkan::Surfaces;
using namespace Entropy::Graphics::Vulkan::QueueFamilies;

ImageView::ImageView(VkImage image, const VkFormat format) {
    const ServiceLocator *sl = ServiceLocator::GetInstance();
    _logicalDevice = sl->getService<ILogicalDevice>();

    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VK_CHECK(vkCreateImageView(_logicalDevice->Get(), &viewInfo, nullptr, &_imageView));
}

ImageView::ImageView(VkImage image,
                     const VkFormat format, const uint32_t flags) {
    const ServiceLocator *sl = ServiceLocator::GetInstance();
    _logicalDevice = sl->getService<ILogicalDevice>();

    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = flags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VK_CHECK(vkCreateImageView(_logicalDevice->Get(), &viewInfo, nullptr, &_imageView));
}

ImageView::~ImageView() {
    vkDestroyImageView(_logicalDevice->Get(), _imageView, nullptr);
}
