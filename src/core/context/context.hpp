/**
 * @file context.hpp
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

#ifdef BUILD_FOR_IOS
#define VK_USE_PLATFORM_IOS_MVK
#include <vulkan/vulkan.hpp>
#include <MetalKit/MetalKit.hpp>
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

#include <plog/Log.h>

// @todo iOS needs version 13.0 for theese?
#include <cstdint>   // Necessary for uint32_t
#include <limits>    // Necessary for std::numeric_limits
#include <algorithm> // Necessary for std::clamp
#include <optional>
#include <set>

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
            /**
             * @brief
             *
             */
            struct QueueFamilyIndices
            {
                std::optional<uint32_t> graphicsFamily;
                std::optional<uint32_t> presentFamily;

                bool isComplete()
                {
                    return graphicsFamily.has_value() && presentFamily.has_value();
                }
            };

            /**
             * @brief
             *
             */
            struct SwapChainSupportDetails
            {
                VkSurfaceCapabilitiesKHR capabilities;
                std::vector<VkSurfaceFormatKHR> formats;
                std::vector<VkPresentModeKHR> presentModes;
            };

#ifdef BUILD_FOR_IOS
            /**
             * @brief Construct a new Context object
             *
             * @param layer
             * @param frame
             */
            Context(CA::MetalLayer *layer, CGRect frame);

            /**
             * @brief Create a surface
             *
             * @param layer
             */
            void CreateSurfaceiOS(CA::MetalLayer *layer);
#endif

#ifdef BUILD_FOR_MACOS
            /**
             * @brief Construct a new Context object
             *
             * @param window
             */
            Context(GLFWwindow *window);

            /**
             * @brief Create a Surface Mac O S object
             *
             * @param window
             */
            void CreateSurfaceMacOS(GLFWwindow *window);
#endif

#ifdef BUILD_FOR_WINDOWS
            /**
             * @brief Construct a new Context object
             *
             * @param window
             */
            Context(GLFWwindow *window);

            /**
             * @brief Create a Surface Windows object
             *
             * @param window
             */
            void CreateSurfaceWindows(GLFWwindow *window);
#endif

#ifdef BUILD_FOR_LINUX
            /**
             * @brief Construct a new Context object
             *
             * @param window
             */
            Context(GLFWwindow *window);

            /**
             * @brief Create a Surface Linux object
             *
             * @param window
             */
            void CreateSurfaceLinux(GLFWwindow *window);
#endif

            /**
             * @brief Destroy the Context object
             *
             */
            ~Context();

            /**
             * @brief Get the Logical Device object
             *
             * @return VkDevice
             */
            VkDevice GetLogicalDevice() { return this->_device; };

            /**
             * @brief Get the Swap Chain object
             *
             * @return VkSwapchainKHR
             */
            VkSwapchainKHR GetSwapChain() { return this->swapChain; };

            /**
             * @brief Get the Swap Chain Extent object
             *
             * @return VkExtent2D
             */
            VkExtent2D GetSwapChainExtent() { return this->swapChainExtent; };

            /**
             * @brief Get the Swap Chain Image Format object
             *
             * @return VkFormat
             */
            VkFormat GetSwapChainImageFormat() { return this->swapChainImageFormat; };

            /**
             * @brief Get the Physical Device object
             *
             * @return VkPhysicalDevice
             */
            VkPhysicalDevice GetPhysicalDevice() { return this->_physicalDevice; };

            /**
             * @brief Get the Swap Chain Image Views object
             *
             * @return std::vector<VkImageView>
             */
            std::vector<VkImageView> GetSwapChainImageViews() { return this->swapChainImageViews; };

            /**
             * @brief
             *
             * @param device
             * @return QueueFamilyIndices
             */
            QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

            // Queues
            VkQueue _graphicsQueue;
            VkQueue _presentQueue;

        private:
            /**
             * @brief Create a Instance object
             *
             */
            void CreateInstance();

            /**
             * @brief
             *
             * @return true
             * @return false
             */
            bool CheckValidationLayerSupport();

            /**
             * @brief Create a Debug Utils Messenger E X T object
             *
             * @param instance
             * @param pCreateInfo
             * @param pAllocator
             * @param pDebugMessenger
             * @return VkResult
             */
            VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger);

            /**
             * @brief
             *
             * @param instance
             * @param debugMessenger
             * @param pAllocator
             */
            void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator);

            /**
             * @brief
             *
             */
            void PickPhysicalDevice();

            /**
             * @brief
             *
             * @param device
             * @return true
             * @return false
             */
            bool IsDeviceSuitable(VkPhysicalDevice device);

            /**
             * @brief Create a Logical Device object
             *
             */
            void CreateLogicalDevice();

            /**
             * @brief Create a Image Views object
             *
             */
            void CreateImageViews();

            /**
             * @brief
             *
             * @param device
             * @return true
             * @return false
             */
            bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

            /**
             * @brief
             *
             * @param device
             * @return SwapChainSupportDetails
             */
            SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

            /**
             * @brief
             *
             * @param availableFormats
             * @return VkSurfaceFormatKHR
             */
            VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);

            /**
             * @brief
             *
             * @param availablePresentModes
             * @return VkPresentModeKHR
             */
            VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);

#ifdef BUILD_FOR_IOS
            /**
             * @brief
             *
             * @param capabilities
             * @param frame
             * @return VkExtent2D
             */
            VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, CGRect frame);

            /**
             * @brief Create a Swap Chain object
             *
             * @param frame
             */
            void CreateSwapChain(CGRect frame);
#endif

#if defined(BUILD_FOR_WINDOWS) || defined(BUILD_FOR_LINUX) || defined(BUILD_FOR_MACOS)
            /**
             * @brief
             *
             * @param capabilities
             * @param window
             * @return VkExtent2D
             */
            VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, GLFWwindow *window);

            /**
             * @brief Create a Swap Chain object
             *
             * @param window
             */
            void CreateSwapChain(GLFWwindow *window);
#endif

            const std::vector<const char *> deviceExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME};

            const std::vector<const char *> validationLayers = {
                "VK_LAYER_KHRONOS_validation"};

            VkSurfaceKHR _surface;

            std::vector<VkImage> swapChainImages;
            VkFormat swapChainImageFormat;
            VkExtent2D swapChainExtent;
            std::vector<VkImageView> swapChainImageViews;

            // Instance and Debugmessenger
            VkInstance _instance;
            VkDebugUtilsMessengerEXT _debugMessenger;

            // Devices
            VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
            VkDevice _device;

            // SwapChain
            VkSwapchainKHR swapChain;
        };
    }
}
