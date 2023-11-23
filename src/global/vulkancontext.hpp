#pragma once

#include <plog/Log.h>
#include <string>
#include <mutex>

#include <contexts/utilities/vulkanutil.hpp>

using namespace Entropy::Contexts::VulkanUtilities;

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

#else
            const std::vector<const char *> _deviceExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME};
#endif

#ifdef BUILD_FOR_MACOS
            inline void CreateSurfaceMacOS(GLFWwindow *window)
            {
                if (glfwCreateWindowSurface(this->_instance, window, nullptr, &_windowSurface) != VK_SUCCESS)
                {
                    PLOG_FATAL << "Could not create MacOS surface!";
                    exit(EXIT_FAILURE);
                }
            }
#endif

            const std::vector<const char *> _validationLayers = {
                "VK_LAYER_KHRONOS_validation"};

            void CreateInstance();

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
                // vkDestroyImage(_device, _textureImage, nullptr);

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
            VkInstance _instance;
            VkDebugUtilsMessengerEXT _debugMessenger;

            // Window surface
            VkSurfaceKHR _windowSurface;

            // Debug messenger & validation layers
            VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger);
            void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator);
            bool CheckValidationLayerSupport();

        public:
            VulkanContext(VulkanContext &other) = delete;
            void operator=(const VulkanContext &) = delete;

            static VulkanContext *GetInstance();
            void RecreateSwapChain(VkExtent2D frame);
            void Initialize(VkExtent2D frame, GLFWwindow *window);
            static VkImageView CreateImageView(VkImage image, VkFormat format);

            // Raw vulkan device objects
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

            // CommandBuffers
            VkCommandPool commandPool;
        };
    }
}
