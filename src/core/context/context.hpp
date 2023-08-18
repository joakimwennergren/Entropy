#pragma once

#include "config.h"

#if BUILD_FOR_IOS == true
#define VK_USE_PLATFORM_IOS_MVK
#include <MetalKit/MetalKit.hpp>
#endif

#if BUILD_FOR_DESKTOP == true
#include <GLFW/glfw3.h>
#endif

#include <vulkan/vulkan.hpp>
#include <plog/Log.h>

#include <iostream>

#include <optional>
#include <set>
#include <cstdint>   // Necessary for uint32_t
#include <limits>    // Necessary for std::numeric_limits
#include <algorithm> // Necessary for std::clamp

namespace Symbios
{
    namespace Core
    {
        /**
         * @brief
         *
         */
        class Context
        {
        public:
            Context() = default;

#if BUILD_FOR_IOS == true
            Context(CA::MetalLayer *layer);
#endif
#if BUILD_FOR_DESKTOP == true
            Context(GLFWwindow *window);
#endif

            ~Context();

#if BUILD_FOR_IOS == true
            // Surface
            void CreateSurfaceiOS(CA::MetalLayer *layer);
#endif

#if BUILD_FOR_MACOS == true
            void CreateSurfaceMacOS(GLFWwindow *window);
#endif

        private:
            void CreateInstance();

        private:
            // Validation Layers
            bool CheckValidationLayerSupport();
            VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger);
            void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator);

            // Physical Device
            void PickPhysicalDevice();
            bool IsDeviceSuitable(VkPhysicalDevice device);

            // Logical device
            void CreateLogicalDevice();

            // Queues
            struct QueueFamilyIndices
            {
                std::optional<uint32_t> graphicsFamily;
                std::optional<uint32_t> presentFamily;

                bool isComplete()
                {
                    return graphicsFamily.has_value() && presentFamily.has_value();
                }
            };
            QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

            // SwapChain
            struct SwapChainSupportDetails
            {
                VkSurfaceCapabilitiesKHR capabilities;
                std::vector<VkSurfaceFormatKHR> formats;
                std::vector<VkPresentModeKHR> presentModes;
            };

        private:
            // Instance and Debugmessenger
            VkInstance _instance;
            VkDebugUtilsMessengerEXT _debugMessenger;

            // Devices
            VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
            VkDevice _device;

            // Queues
            VkQueue _graphicsQueue;
            VkQueue _presentQueue;

            // SwapChain
            VkSwapchainKHR swapChain;

            const std::vector<const char *> deviceExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME};

            bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
            SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
            VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
            VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
            VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);
            void CreateSwapChain();

            // Surface
            VkSurfaceKHR _surface;
        };
    }

}
