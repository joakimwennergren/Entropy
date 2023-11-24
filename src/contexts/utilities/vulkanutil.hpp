/**
 * @file utilities.hpp
 * @author Joakim Wennergren (joakim.wennergren@databeams.se)
 * @brief
 * @version 0.1
 * @date 2023-08-22
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include "config.hpp"

// @todo iOS needs version 13.0 for theese?
#include <cstdint>   // Necessary for uint32_t
#include <limits>    // Necessary for std::numeric_limits
#include <algorithm> // Necessary for std::clamp
#include <optional>
#include <set>

#ifdef BUILD_FOR_IOS
#define VK_USE_PLATFORM_IOS_MVK
#include <vulkan/vulkan.hpp>
#include <MetalKit/MetalKit.hpp>
#endif

#ifdef BUILD_FOR_ANDROID
#include <vulkan/vulkan.hpp>
#endif

#ifdef BUILD_FOR_MACOS
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#endif

#ifdef BUILD_FOR_WINDOWS
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#undef max
#endif

#ifdef BUILD_FOR_LINUX
#include <vulkan/vulkan.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#endif

namespace Entropy
{
    namespace Contexts
    {
        namespace VulkanUtilities
        {
            struct QueueFamilyIndices
            {
                std::optional<uint32_t> graphicsFamily;
                std::optional<uint32_t> presentFamily;

                bool isComplete()
                {
                    return graphicsFamily.has_value() && presentFamily.has_value();
                }
            };

            struct SwapChainSupportDetails
            {
                VkSurfaceCapabilitiesKHR capabilities;
                std::vector<VkSurfaceFormatKHR> formats;
                std::vector<VkPresentModeKHR> presentModes;
            };

            class VulkanUtility
            {
            public:
                static SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
                static bool IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, const std::vector<const char *> deviceExtensions);
                static bool CheckDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char *> extensions);
                static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
                static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
                static VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
                static VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, VkExtent2D frame);
            };
        }
    }
}
