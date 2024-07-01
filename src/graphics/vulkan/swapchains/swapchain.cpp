#include "swapchain.hpp"
#include "vulkan/vulkan_core.h"

using namespace Entropy::Graphics::Vulkan::Swapchains;

// Swapchain::Swapchain(VkPhysicalDevice physicalDevice, VkDevice logicalDevice,
//                      std::shared_ptr<WindowSurface> surface, VkExtent2D frame) {
//   _logicalDevice = logicalDevice;
//   _surface = surface;
//   _physicalDevice = physicalDevice;
//   _frame = frame;
//   CreateSwapChain(nullptr);
// }

// Swapchain::Swapchain(VkPhysicalDevice physicalDevice, VkDevice logicalDevice,
//                      VkExtent2D frame, int width, int height) {
//   _logicalDevice = logicalDevice;
//   _physicalDevice = physicalDevice;
//   _frame = frame;

//   VkImageCreateInfo imageInfo{};
//   imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
//   imageInfo.imageType = VK_IMAGE_TYPE_2D;
//   imageInfo.extent.width = width;
//   imageInfo.extent.height = height;
//   imageInfo.extent.depth = 1;
//   imageInfo.mipLevels = 1;
//   imageInfo.arrayLayers = 1;
//   imageInfo.format = VK_FORMAT_B8G8R8A8_SRGB;
//   imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
//   imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//   imageInfo.usage =
//       VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
//   imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
//   imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

//   swapChainImages.resize(1);

//   vkCreateImage(_logicalDevice, &imageInfo, nullptr, &swapChainImages[0]);

//   VkMemoryRequirements memRequirements;
//   vkGetImageMemoryRequirements(_logicalDevice, swapChainImages[0],
//                                &memRequirements);

//   VkMemoryAllocateInfo allocInfo{};
//   allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
//   allocInfo.allocationSize = memRequirements.size;
//   allocInfo.memoryTypeIndex =
//       FindMemoryTypeIndex(_physicalDevice, memRequirements.memoryTypeBits,
//                           VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

//   if (vkAllocateMemory(_logicalDevice, &allocInfo, nullptr,
//                        &_renderImageMemory) != VK_SUCCESS) {
//     throw std::runtime_error("failed to allocate image memory!");
//   }

//   vkBindImageMemory(_logicalDevice, swapChainImages[0], _renderImageMemory, 0);

//   swapChainImageFormat = VK_FORMAT_B8G8R8A8_SRGB;
//   swapChainImageViews.resize(swapChainImages.size());
//   swapChainExtent =
//       VkExtent2D{static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

//   for (uint32_t i = 0; i < swapChainImages.size(); i++) {
//     auto imageView =
//         ImageView(_logicalDevice, swapChainImages[i], swapChainImageFormat);
//     swapChainImageViews[i] = imageView.Get();
//   }
// }

// Swapchain::~Swapchain() {
//   vkDestroySwapchainKHR(_logicalDevice, _swapChain, nullptr);
// }

// void Swapchain::CreateSwapChain(VkSwapchainKHR prev) {
//   SwapChainSupportDetails swapChainSupport =
//       QuerySwapChainSupport(_physicalDevice, _surface->Get());
//   VkSurfaceFormatKHR surfaceFormat =
//       ChooseSwapSurfaceFormat(swapChainSupport.formats);
//   VkPresentModeKHR presentMode =
//       ChooseSwapPresentMode(swapChainSupport.presentModes);
//   VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities, _frame);

//   uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

//   if (swapChainSupport.capabilities.maxImageCount > 0 &&
//       imageCount > swapChainSupport.capabilities.maxImageCount) {
//     imageCount = swapChainSupport.capabilities.maxImageCount;
//   }

//   VkSwapchainCreateInfoKHR createInfo{};
//   createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
//   createInfo.surface = _surface->Get();
//   createInfo.minImageCount = imageCount;
//   createInfo.imageFormat = surfaceFormat.format;
//   createInfo.imageColorSpace = surfaceFormat.colorSpace;
//   createInfo.imageExtent = extent;
//   createInfo.imageArrayLayers = 1;
//   createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

//   QueueFamilyIndices indices =
//       QueueFamily::FindQueueFamilies(_physicalDevice, _surface->Get());
//   uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(),
//                                    indices.presentFamily.value()};

//   if (indices.graphicsFamily != indices.presentFamily) {
//     createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
//     createInfo.queueFamilyIndexCount = 2;
//     createInfo.pQueueFamilyIndices = queueFamilyIndices;
//   } else {
//     createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
//     createInfo.queueFamilyIndexCount = 0;     // Optional
//     createInfo.pQueueFamilyIndices = nullptr; // Optional
//   }
//   createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
// #ifdef BUILD_FOR_WINDOWS
//   createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
// #else
//   createInfo.compositeAlpha =
//       VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR; // VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
// #endif
//   createInfo.presentMode = presentMode;
//   createInfo.clipped = VK_TRUE;
//   createInfo.oldSwapchain = prev == nullptr ? VK_NULL_HANDLE : prev;

//   if (vkCreateSwapchainKHR(_logicalDevice, &createInfo, nullptr, &_swapChain) !=
//       VK_SUCCESS) {
//     exit(EXIT_FAILURE);
//   }

//   vkDestroySwapchainKHR(_logicalDevice, prev, nullptr);

//   vkGetSwapchainImagesKHR(_logicalDevice, _swapChain, &imageCount, nullptr);
//   swapChainImages.resize(imageCount);
//   vkGetSwapchainImagesKHR(_logicalDevice, _swapChain, &imageCount,
//                           swapChainImages.data());

//   swapChainImageFormat = surfaceFormat.format;
//   swapChainExtent = extent;

//   swapChainImageViews.resize(swapChainImages.size());

//   for (uint32_t i = 0; i < swapChainImages.size(); i++) {
//     auto imageView =
//         ImageView(_logicalDevice, swapChainImages[i], swapChainImageFormat);
//     swapChainImageViews[i] = imageView.Get();
//   }
// }

// void Swapchain::RecreateSwapChain(int width, int height) {

//   vkDeviceWaitIdle(_logicalDevice);

//   for (size_t i = 0; i < swapChainImageViews.size(); i++) {
//     vkDestroyImageView(_logicalDevice, swapChainImageViews[i], nullptr);
//   }

//   VkImageCreateInfo imageInfo{};
//   imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
//   imageInfo.imageType = VK_IMAGE_TYPE_2D;
//   imageInfo.extent.width = width;
//   imageInfo.extent.height = height;
//   imageInfo.extent.depth = 1;
//   imageInfo.mipLevels = 1;
//   imageInfo.arrayLayers = 1;
//   imageInfo.format = VK_FORMAT_B8G8R8A8_SRGB;
//   imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
//   imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//   imageInfo.usage =
//       VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
//   imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
//   imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

//   swapChainImages.resize(1);

//   vkCreateImage(_logicalDevice, &imageInfo, nullptr, &swapChainImages[0]);

//   VkMemoryRequirements memRequirements;
//   vkGetImageMemoryRequirements(_logicalDevice, swapChainImages[0],
//                                &memRequirements);

//   VkMemoryAllocateInfo allocInfo{};
//   allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
//   allocInfo.allocationSize = memRequirements.size;
//   allocInfo.memoryTypeIndex =
//       FindMemoryTypeIndex(_physicalDevice, memRequirements.memoryTypeBits,
//                           VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

//   if (vkAllocateMemory(_logicalDevice, &allocInfo, nullptr,
//                        &_renderImageMemory) != VK_SUCCESS) {
//     throw std::runtime_error("failed to allocate image memory!");
//   }

//   vkBindImageMemory(_logicalDevice, swapChainImages[0], _renderImageMemory, 0);

//   swapChainImageFormat = VK_FORMAT_B8G8R8A8_SRGB;
//   swapChainImageViews.resize(swapChainImages.size());
//   swapChainExtent =
//       VkExtent2D{static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

//   for (uint32_t i = 0; i < swapChainImages.size(); i++) {
//     auto imageView =
//         ImageView(_logicalDevice, swapChainImages[i], swapChainImageFormat);
//     swapChainImageViews[i] = imageView.Get();
//   }
// }

// void Swapchain::RecreateSwapChain() {
//   vkDeviceWaitIdle(_logicalDevice);

//   for (size_t i = 0; i < swapChainImageViews.size(); i++) {
//     vkDestroyImageView(_logicalDevice, swapChainImageViews[i], nullptr);
//   }

//   CreateSwapChain(_swapChain);
// }

// SwapChainSupportDetails
// Swapchain::QuerySwapChainSupport(VkPhysicalDevice device,
//                                  VkSurfaceKHR surface) {
//   SwapChainSupportDetails details;

//   vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface,
//                                             &details.capabilities);

//   uint32_t formatCount;
//   vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

//   if (formatCount != 0) {
//     details.formats.resize(formatCount);
//     vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount,
//                                          details.formats.data());
//   }

//   uint32_t presentModeCount;
//   vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount,
//                                             nullptr);

//   if (presentModeCount != 0) {
//     details.presentModes.resize(presentModeCount);
//     vkGetPhysicalDeviceSurfacePresentModesKHR(
//         device, surface, &presentModeCount, details.presentModes.data());
//   }

//   return details;
// }

// VkSurfaceFormatKHR Swapchain::ChooseSwapSurfaceFormat(
//     const std::vector<VkSurfaceFormatKHR> &availableFormats) {
// #if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_LINUX)
//   auto colorFormat = VK_FORMAT_R8G8B8A8_SRGB;

// #elif defined(BUILD_FOR_WINDOWS)
//   auto colorFormat = VK_FORMAT_R8G8B8A8_UNORM;

// #else
//   auto colorFormat = VK_FORMAT_B8G8R8A8_SRGB;
// #endif

//   for (const auto &availableFormat : availableFormats) {
//     if (availableFormat.format == colorFormat &&
//         availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
//       return availableFormat;
//     }
//   }

//   return availableFormats[0];
// }

// VkPresentModeKHR Swapchain::ChooseSwapPresentMode(
//     const std::vector<VkPresentModeKHR> &availablePresentModes) {
//   for (const auto &availablePresentMode : availablePresentModes) {
//     if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
//       return availablePresentMode;
//     }
//   }

//   return VK_PRESENT_MODE_FIFO_KHR;
// }

// VkExtent2D
// Swapchain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities,
//                             VkExtent2D frame) {
//   if (capabilities.currentExtent.width !=
//       std::numeric_limits<uint32_t>::max()) {
//     return capabilities.currentExtent;
//   } else {
//     VkExtent2D actualExtent = {static_cast<uint32_t>(frame.width),
//                                static_cast<uint32_t>(frame.height)};

//     actualExtent.width =
//         std::clamp(actualExtent.width, capabilities.minImageExtent.width,
//                    capabilities.maxImageExtent.width);
//     actualExtent.height =
//         std::clamp(actualExtent.height, capabilities.minImageExtent.height,
//                    capabilities.maxImageExtent.height);

//     return actualExtent;
//   }
// }