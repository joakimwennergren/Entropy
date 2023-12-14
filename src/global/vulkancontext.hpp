#pragma once

#include <string>
#include <mutex>
#include <iostream>

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_LINUX) || defined(BUILD_FOR_WINDOWS)
#include <GLFW/glfw3.h>
#endif

#include <utilities/vulkanutil.hpp>

using namespace Entropy::Utilities::VulkanUtilities;

namespace Entropy
{
    namespace Global
    {
        class VulkanContext
        {

        private:
            static VulkanContext *pinstance_;
            static std::mutex mutex_;

#ifdef BUILD_FOR_MACOS
            const std::vector<const char *> _deviceExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME, "VK_KHR_portability_subset"};
#endif

#ifdef BUILD_FOR_ANDROID
            const std::vector<const char *> _deviceExtensions = {
                "VK_KHR_swapchain"};
#endif

#ifdef BUILD_FOR_MACOS
            void CreateSurfaceMacOS(GLFWwindow *window);
#endif

            const std::vector<const char *> _validationLayers = {
                "VK_LAYER_KHRONOS_validation"};

            void CreateLogicalDevice();
            void PickPhysicalDevice();

            // Descriptorsets
            void CreateDescriptorPool();
            void CreateDesciptorsetLayout();
            void CreateDescriptorSets();

            // CommandBuffers
            void CreateCommandPool();

            // SwapChain
            void CreateSwapChain(VkExtent2D frame);

            // ImageViews??
            void CreateImageViews();

        protected:
            VulkanContext(){};
            ~VulkanContext()
            {
                vkDestroyDescriptorPool(logicalDevice, descriptorPool, nullptr);
                vkDestroyDescriptorSetLayout(logicalDevice, descriptorSetLayout, nullptr);

                vkDestroyCommandPool(logicalDevice, commandPool, nullptr);

                for (auto imageView : swapChainImageViews)
                {
                    vkDestroyImageView(logicalDevice, imageView, nullptr);
                }
                vkDestroySwapchainKHR(logicalDevice, swapChain, nullptr);
                vkDestroyDevice(logicalDevice, nullptr);
                vkDestroySurfaceKHR(_instance, _windowSurface, nullptr);

#ifdef USE_VALIDATION_LAYERS
                DestroyDebugUtilsMessengerEXT(_instance, _debugMessenger, nullptr);
#endif

                // Last
                vkDestroyInstance(_instance, nullptr);
            }

            // Instance and Debugmessenger
            VkDebugUtilsMessengerEXT _debugMessenger;

            // Window surface

            // Debug messenger & validation layers
            VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger);
            void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator);
            bool CheckValidationLayerSupport();

        public:
            VulkanContext(VulkanContext &other) = delete;
            void operator=(const VulkanContext &) = delete;

            static VulkanContext *GetInstance();
            void RecreateSwapChain(VkExtent2D frame);
#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_LINUX) || defined(BUILD_FOR_WINDOWS)
            void Initialize(VkExtent2D frame, GLFWwindow *window);
#endif
#if defined(BUILD_FOR_ANDROID)
            VkInstance Initialize(VkInstance instance);
#endif
#if defined(BUILD_FOR_IOS)
            void Initialize(VkExtent2D frame);
#endif

            static VkImageView CreateImageView(VkImage image, VkFormat format);

            // Devices
            VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
            VkDevice logicalDevice = VK_NULL_HANDLE;

            // Queues
            VkQueue graphicsQueue;
            VkQueue presentQueue;

            // SwapChain
            VkSwapchainKHR swapChain;
            VkExtent2D swapChainExtent;
            VkFormat swapChainImageFormat;
            std::vector<VkImageView> swapChainImageViews;
            std::vector<VkImage> swapChainImages;

            // DescriptorSets
            std::vector<VkDescriptorSet> descriptorSets;
            VkDescriptorSetLayout descriptorSetLayout;
            VkDescriptorPool descriptorPool;

            // Command pool
            VkCommandPool commandPool;

            VkSurfaceKHR _windowSurface;

            VkInstance _instance;

            VkInstance CreateInstance();
        };
    }
}
