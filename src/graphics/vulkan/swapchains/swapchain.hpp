#pragma once

#include "vulkan/vulkan_core.h"
#include <config.hpp>
#include <iostream>
#include <limits>
#include <vulkan/vulkan.hpp>

#include "iswapchain.hpp"

#include <graphics/vulkan/imageviews/imageview.hpp>
#include <graphics/vulkan/queuefamilies/queuefamily.hpp>
#include <graphics/vulkan/surfaces/surface.hpp>

#include <graphics/vulkan/devices/ilogical_device.hpp>
#include <graphics/vulkan/devices/iphysical_device.hpp>
#include <graphics/vulkan/utilities/utilities.hpp>

using namespace Entropy::Graphics::Vulkan::Surfaces;
using namespace Entropy::Graphics::Vulkan::QueueFamilies;
using namespace Entropy::Graphics::Vulkan::ImageViews;

namespace Entropy::Graphics::Vulkan::SwapChains {
  /**
   * @brief Represents the details of swap chain support for a Vulkan physical device.
   *
   * This class contains information about the swap chain capabilities, formats,
   * and present modes supported by a Vulkan physical device. It is used to assess
   * and configure the swap chain for rendering.
   */
  struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities{};
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
  };

  struct SwapChain final : ServiceBase<ISwapchain> {
    /**
     * @brief Constructs a SwapChain object and initializes Vulkan device-specific members.
     *
     * This constructor retrieves essential Vulkan device interfaces via a ServiceLocator
     * instance and sets up the initial swap chain image format and extent.
     *
     * @return A newly constructed SwapChain object.
     */
    SwapChain() {
      const ServiceLocator *sl = ServiceLocator::GetInstance();
      _physicalDevice = sl->getService<IPhysicalDevice>();
      _logicalDevice = sl->getService<ILogicalDevice>();

      swapChainImageFormat = VK_FORMAT_B8G8R8A8_SRGB;
      swapChainExtent = VkExtent2D{
        static_cast<uint32_t>(320 * 10),
        static_cast<uint32_t>(480 * 6)
      };
    }

    /**
     * @brief Recreates the Vulkan swap chain with the specified dimensions and application state.
     *
     * This method updates the swap chain image format, extent, and clears existing swap chain images and image views.
     * If the application state is true, it calls the Build function to rebuild the swap chain.
     *
     * @param width The new width for the swap chain extent.
     * @param height The new height for the swap chain extent.
     * @param app The application state that determines whether to rebuild the swap chain.
     */
    void RecreateSwapChain(const int width, const int height) override {
      swapChainImageFormat = VK_FORMAT_B8G8R8A8_UNORM;
      swapChainExtent =
          VkExtent2D{static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
      _swapChainImageViews.clear();
      _swapChainImages.clear();
      Build(_surface, swapChainExtent, _swapChain);
    }

    void Build(VkSurfaceKHR surface, const VkExtent2D frame,
               VkSwapchainKHR oldSwapChain) override {
      _surface = surface;

      swapChainImageFormat = VK_FORMAT_B8G8R8A8_UNORM;
      swapChainExtent =
          VkExtent2D{static_cast<uint32_t>(320), static_cast<uint32_t>(480)};
      auto [capabilities, formats, presentModes] =
          QuerySwapChainSupport(_physicalDevice->Get(), surface);
      auto [format, colorSpace] =
          ChooseSwapSurfaceFormat(formats);
      VkPresentModeKHR presentMode =
          ChooseSwapPresentMode(presentModes);
      VkExtent2D extent = ChooseSwapExtent(capabilities, frame);

      uint32_t imageCount = capabilities.minImageCount + 1;

      if (capabilities.maxImageCount > 0 &&
          imageCount > capabilities.maxImageCount) {
        imageCount = capabilities.maxImageCount;
      }

      VkSwapchainCreateInfoKHR createInfo{};
      createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
      createInfo.surface = surface;
      createInfo.minImageCount = imageCount;
      createInfo.imageFormat = format;
      createInfo.imageColorSpace = colorSpace;
      createInfo.imageExtent = extent;
      createInfo.imageArrayLayers = 1;
      createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

      QueueFamilyIndices indices =
          QueueFamily::FindQueueFamilies(_physicalDevice->Get(), surface);
      uint32_t queueFamilyIndices[] = {
        indices.graphicsFamily.value(),
        indices.presentFamily.value()
      };

      if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
      } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional
      }
      createInfo.preTransform = capabilities.currentTransform;
#ifdef BUILD_FOR_WINDOWS
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
#else
      createInfo.compositeAlpha =
          VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR; // VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
#endif
      createInfo.presentMode = presentMode;
      createInfo.clipped = VK_TRUE;
      createInfo.oldSwapchain = oldSwapChain;

      VK_CHECK(vkCreateSwapchainKHR(_logicalDevice->Get(), &createInfo, nullptr,
        &_swapChain));

      swapChainImageFormat = format;
      swapChainExtent = extent;

      imageCount = capabilities.minImageCount + 1;

      vkGetSwapchainImagesKHR(_logicalDevice->Get(), _swapChain, &imageCount,
                              nullptr);

      _swapChainImages.resize(imageCount);

      vkGetSwapchainImagesKHR(_logicalDevice->Get(), _swapChain, &imageCount,
                              _swapChainImages.data());

      _swapChainImageViews.resize(_swapChainImages.size());

      for (uint32_t i = 0; i < _swapChainImages.size(); i++) {
        auto imageView = ImageView(_swapChainImages[i], swapChainImageFormat);
        _swapChainImageViews[i] = imageView.Get();
      }
    }

    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device,
                                                  VkSurfaceKHR surface) {
      SwapChainSupportDetails details;

      vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface,
                                                &details.capabilities);

      uint32_t formatCount;
      vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount,
                                           nullptr);

      if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount,
                                             details.formats.data());
      }

      uint32_t presentModeCount;
      vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface,
                                                &presentModeCount, nullptr);

      if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(
          device, surface, &presentModeCount, details.presentModes.data());
      }

      return details;
    }

    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(
      const std::vector<VkSurfaceFormatKHR> &availableFormats) {
      // #if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_LINUX)
      //     auto colorFormat = VK_FORMAT_R8G8B8A8_SRGB;

      // #elif defined(BUILD_FOR_WINDOWS)
      //     auto colorFormat = VK_FORMAT_R8G8B8A8_UNORM;

      // #else
      //     auto colorFormat = VK_FORMAT_B8G8R8A8_SRGB;
      // #endif

      auto colorFormat = VK_FORMAT_R8G8B8A8_UNORM;

      for (const auto &availableFormat: availableFormats) {
        if (availableFormat.format == colorFormat &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
          return availableFormat;
        }
      }

      return availableFormats[0];
    }

    VkPresentModeKHR ChooseSwapPresentMode(
      const std::vector<VkPresentModeKHR> &availablePresentModes) {
      for (const auto &availablePresentMode: availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
          return availablePresentMode;
        }
      }

      return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities,
                                VkExtent2D frame) {
      if (capabilities.currentExtent.width !=
          std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
      } else {
        VkExtent2D actualExtent = {
          static_cast<uint32_t>(frame.width),
          static_cast<uint32_t>(frame.height)
        };

        actualExtent.width =
            std::clamp(actualExtent.width, capabilities.minImageExtent.width,
                       capabilities.maxImageExtent.width);
        actualExtent.height =
            std::clamp(actualExtent.height, capabilities.minImageExtent.height,
                       capabilities.maxImageExtent.height);

        return actualExtent;
      }
    }

    // static SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice
    // device,
    //                                                      VkSurfaceKHR surface);
    // std::vector<VkImageView> swapChainImageViews;

    inline VkSwapchainKHR Get() { return _swapChain; };
    // std::shared_ptr<WindowSurface> _surface;

  private:
    std::shared_ptr<IPhysicalDevice> _physicalDevice;
    std::shared_ptr<ILogicalDevice> _logicalDevice;
    VkSurfaceKHR _surface = VK_NULL_HANDLE;

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
} // namespace Entropy::Graphics::Vulkan::Swapchains



