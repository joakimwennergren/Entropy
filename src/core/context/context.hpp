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

#include "mvpuniform.hpp"
#include "instance.hpp"

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
            Context();

            void setLayerAndFrame(CA::MetalLayer *layer, CGRect frame);

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
            VkSwapchainKHR GetSwapChain() { return this->_swapChain; };

            /**
             * @brief Get the Swap Chain Extent object
             *
             * @return VkExtent2D
             */
            VkExtent2D GetSwapChainExtent() { return this->_swapChainExtent; };

            /**
             * @brief Get the Swap Chain Image Format object
             *
             * @return VkFormat
             */
            VkFormat GetSwapChainImageFormat() { return this->_swapChainImageFormat; };

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
            std::vector<VkImageView> GetSwapChainImageViews() { return this->_swapChainImageViews; };

            /**
             * @brief Get the Graphics Queue object
             *
             * @return VkQueue
             */
            VkQueue GetGraphicsQueue() { return this->_graphicsQueue; };

            /**
             * @brief Get the Present Queue object
             *
             * @return VkQueue
             */
            VkQueue GetPresentQueue() { return this->_presentQueue; };

            /**
             * @brief Get the Command Pool object
             *
             * @return VkCommandPool
             */
            VkCommandPool GetCommandPool() { return this->_commandPool; };

            /**
             * @brief Get the Descriptor Sets object
             *
             * @return std::vector<VkDescriptorSet>
             */
            inline std::vector<VkDescriptorSet> GetDescriptorSets() { return this->_descriptorSets; };

            /**
             * @brief Get the Descriptor Set Layouts object
             *
             * @return VkDescriptorSetLayout
             */
            inline VkDescriptorSetLayout GetDescriptorSetLayouts() { return this->_descriptorSetLayout; };

            /**
             * @brief Get the Descriptor Pool object
             *
             * @return _descriptorPool
             */
            inline VkDescriptorPool GetDescriptorPool() { return this->_descriptorPool; };

            /**
             * @brief
             *
             * @param device
             * @return QueueFamilyIndices
             */
            QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

            /**
             * @brief Create a Descriptor Sets object
             *
             * @param uniformBuffers
             */
            void CreateDescriptorSets(std::vector<VkBuffer> uniformBuffers, VkImageView view);

            /**
             * @brief
             *
             * @param typeFilter
             * @param properties
             * @return uint32_t
             */
            uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

            /**
             * @brief
             *
             * @param window
             */
            void RecreateSwapChain();

            /**
             * @brief Create a Image View object
             *
             * @param image
             * @param format
             * @return VkImageView
             */
            VkImageView CreateImageView(VkImage image, VkFormat format);

            void Test(std::vector<VkBuffer> uniformBuffers, VkImageView imageView);
            std::vector<VkDescriptorImageInfo> descriptorImageInfos;
            // Texture samplers
            VkSampler _textureSampler;

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

            /**
             * @brief Create a Command Pool object
             *
             */
            void CreateCommandPool();

            /**
             * @brief
             *
             */
            void CreateDescriptorPool();

            /**
             * @brief Construct a new void Create Descriptor Set Layout object
             *
             */
            void CreateDescriptorSetLayout();

            /**
             * @brief Create a Texture Image View object
             *
             */
            void CreateTextureImageView();

            /**
             * @brief Create a Texture Sampler object
             *
             */
            void CreateTextureSampler();

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

#ifdef BUILD_FOR_MACOS
            const std::vector<const char *> deviceExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME, "VK_KHR_portability_subset"};

#else
            const std::vector<const char *> deviceExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME};
#endif

            const std::vector<const char *> validationLayers = {
                "VK_LAYER_KHRONOS_validation"};

            VkSurfaceKHR _surface;

            std::vector<VkImage> _swapChainImages;
            VkFormat _swapChainImageFormat;
            VkExtent2D _swapChainExtent;
            std::vector<VkImageView> _swapChainImageViews;

            // Instance and Debugmessenger
            VkInstance _instance;
            VkDebugUtilsMessengerEXT _debugMessenger;

            // Devices
            VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
            VkDevice _device;

            // SwapChain
            VkSwapchainKHR _swapChain;

            // Queues
            VkQueue _graphicsQueue;
            VkQueue _presentQueue;

            // Pools
            VkCommandPool _commandPool;
            VkDescriptorPool _descriptorPool;

            // TextureViews
            VkImageView _textureImageView;
            VkImage _textureImage;

            VkDescriptorSetLayout _descriptorSetLayout;
            std::vector<VkDescriptorSet> _descriptorSets;

            VkDescriptorSet _uboInstanceDS;

#if defined(BUILD_FOR_WINDOWS) || defined(BUILD_FOR_LINUX) || defined(BUILD_FOR_MACOS)
            GLFWwindow *_window;
#endif

#ifdef BUILD_FOR_IOS
            CA::MetalLayer *_layer;
            CGRect _frame;
#endif
        };
    }
}
