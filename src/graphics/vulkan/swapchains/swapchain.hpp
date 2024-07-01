#pragma once

#include <config.hpp>
#include <iostream>
#include <limits>
#include <vulkan/vulkan.hpp>

#include <graphics/vulkan/imageviews/imageview.hpp>
#include <graphics/vulkan/queuefamilies/queuefamily.hpp>
#include <graphics/vulkan/surfaces/surface.hpp>

#include <tracy/Tracy.hpp>
#include <graphics/vulkan/vulkan_backend.hpp>

using namespace Entropy::Graphics::Vulkan::Surfaces;
using namespace Entropy::Graphics::Vulkan::QueueFamilies;
using namespace Entropy::Graphics::Vulkan::ImageViews;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Swapchains {

struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;
};

struct Swapchain {

  Swapchain(VulkanBackend vbe) : _backend{vbe}
  {
  swapChainImageFormat = VK_FORMAT_B8G8R8A8_SRGB;
  swapChainExtent = VkExtent2D{static_cast<uint32_t>(800), static_cast<uint32_t>(800)};
  }
  // Swapchain(VkPhysicalDevice physicalDevice, VkDevice logicalDevice,
  //           std::shared_ptr<WindowSurface> surface, VkExtent2D frame);
  // Swapchain(VkPhysicalDevice physicalDevice, VkDevice logicalDevice,
  //           VkExtent2D frame, int width, int height);
  // ~Swapchain();
  // void CreateSwapChain(VkSwapchainKHR prev);
  // void RecreateSwapChain();
  // void RecreateSwapChain(int width, int height);
  // static SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device,
  //                                                      VkSurfaceKHR surface);
  // std::vector<VkImageView> swapChainImageViews;
  // std::vector<VkImage> swapChainImages;
  VkExtent2D swapChainExtent;
  VkFormat swapChainImageFormat;
  // inline VkSwapchainKHR Get() { return _swapChain; };
  // std::shared_ptr<WindowSurface> _surface;

private:


    Vulkan::VulkanBackend _backend;

  // VkSwapchainKHR _swapChain;
  // VkSurfaceFormatKHR ChooseSwapSurfaceFormat(
  //     const std::vector<VkSurfaceFormatKHR> &availableFormats);
  // VkPresentModeKHR ChooseSwapPresentMode(
  //     const std::vector<VkPresentModeKHR> &availablePresentModes);
  // VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities,
  //                             VkExtent2D frame);
  // VkDevice _logicalDevice;
  // VkPhysicalDevice _physicalDevice;

  // VkExtent2D _frame;
  // VkDeviceMemory _renderImageMemory = nullptr;

  // uint32_t FindMemoryTypeIndex(VkPhysicalDevice physicalDevice,
  //                              uint32_t typeFilter,
  //                              VkMemoryPropertyFlags properties) {

  //   // Get the physical device's memory properties
  //   VkPhysicalDeviceMemoryProperties memProperties;
  //   vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

  //   // Iterate over memoryproperties and return index of matched property
  //   for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
  //     if ((typeFilter & (1 << i)) &&
  //         (memProperties.memoryTypes[i].propertyFlags & properties) ==
  //             properties) {
  //       return i;
  //     }
  //   }

  //   return 0;
  // }
};
} // namespace Swapchains
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy
