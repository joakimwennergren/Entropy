#include "imageview.hpp"

using namespace Entropy::Graphics::Vulkan::ImageViews;

ImageView::ImageView(VkImage image, VkFormat format)
{

  ServiceLocator *sl = ServiceLocator::GetInstance();
  auto logicalDevice = sl->getService<ILogicalDevice>();

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

  if (vkCreateImageView(logicalDevice->Get(), &viewInfo, nullptr, &_imageView) !=
      VK_SUCCESS)
  {
    throw std::runtime_error("failed to create texture image view!");
  }
}

ImageView::ImageView(VkImage depthImage,
                     VkFormat format, uint32_t flags)
{
  ServiceLocator *sl = ServiceLocator::GetInstance();
  auto logicalDevice = sl->getService<ILogicalDevice>();

  VkImageViewCreateInfo viewInfo{};
  viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  viewInfo.image = depthImage;
  viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
  viewInfo.format = format;
  viewInfo.subresourceRange.aspectMask = flags;
  viewInfo.subresourceRange.baseMipLevel = 0;
  viewInfo.subresourceRange.levelCount = 1;
  viewInfo.subresourceRange.baseArrayLayer = 0;
  viewInfo.subresourceRange.layerCount = 1;

  if (vkCreateImageView(logicalDevice->Get(), &viewInfo, nullptr, &_imageView) !=
      VK_SUCCESS)
  {
    throw std::runtime_error("failed to create texture image view!");
  }
}