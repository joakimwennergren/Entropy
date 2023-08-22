#pragma once

#include "config.hpp"

#ifdef BUILD_FOR_IOS
#define VK_USE_PLATFORM_IOS_MVK
#include <MetalKit/MetalKit.hpp>
#endif

#include <vulkan/vulkan.hpp>

#ifdef BUILD_FOR_MACOS
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
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#endif

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
#ifdef BUILD_FOR_IOS
            Context(CA::MetalLayer *layer, CGRect frame);
#endif

#ifdef BUILD_FOR_MACOS
            Context(GLFWwindow *window);
#endif

#ifdef BUILD_FOR_WINDOWS
            Context(GLFWwindow *window);
#endif

#ifdef BUILD_FOR_LINUX
            Context(GLFWwindow *window);
#endif

            ~Context();

#ifdef BUILD_FOR_IOS
            // Surface
            void CreateSurfaceiOS(CA::MetalLayer *layer);
#endif

#ifdef BUILD_FOR_MACOS
            void CreateSurfaceMacOS(GLFWwindow *window);
#endif

#ifdef BUILD_FOR_WINDOWS
            void CreateSurfaceWindows(GLFWwindow *window);
#endif

#ifdef BUILD_FOR_LINUX
            void CreateSurfaceLinux(GLFWwindow *window);
#endif

            // Getters
            VkDevice GetLogicalDevice() { return this->_device; };
            VkSwapchainKHR GetSwapChain() { return this->swapChain; };
            VkExtent2D GetSwapChainExtent() { return this->swapChainExtent; };
            VkFormat GetSwapChainImageFormat() { return this->swapChainImageFormat; };
            VkPhysicalDevice GetPhysicalDevice() { return this->_physicalDevice; };
            std::vector<VkImageView> GetSwapChainImageViews() { return this->swapChainImageViews; };

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

            // Queues
            VkQueue _graphicsQueue;
            VkQueue _presentQueue;

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

            // SwapChain
            VkSwapchainKHR swapChain;

            const std::vector<const char *> deviceExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME};

            bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
            SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
            VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
            VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);

#ifdef BUILD_FOR_IOS
            VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, CGRect frame);
            void CreateSwapChain(CGRect frame);
#endif

#if defined(BUILD_FOR_WINDOWS) || defined(BUILD_FOR_LINUX) || defined(BUILD_FOR_MACOS)
            VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, GLFWwindow *window);
            void CreateSwapChain(GLFWwindow *window);
#endif

            // Surface
            VkSurfaceKHR _surface;

            // Image Views
            std::vector<VkImage> swapChainImages;
            VkFormat swapChainImageFormat;
            VkExtent2D swapChainExtent;
            std::vector<VkImageView> swapChainImageViews;
            void CreateImageViews();
        };
    }
}
