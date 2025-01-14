#ifndef ENTROPY_VULKAN_INSTANCE_H
#define ENTROPY_VULKAN_INSTANCE_H

#include <vulkan/vulkan.hpp>
#include <spdlog/spdlog.h>
#include "ivk_instance.hpp"
#include <graphics/vulkan/validationlayers/validationlayer.hpp>
#include <servicelocators/servicelocator.hpp>
#include <graphics/vulkan/utilities/helpers.hpp>

namespace Entropy::Graphics::Vulkan::Instances {
  struct VulkanInstance final : ServiceBase<IVulkanInstance> {
    /**
     * @brief Constructs a new VulkanInstance object.
     *
     * This constructor initializes the Vulkan instance with the specified application and engine
     * information, enables required extensions, and sets up the validation layers and debugging
     * utilities based on the build configuration (e.g., for MacOS or Android).
     *
     * @return A VulkanInstance object with initialized Vulkan instance and debug utilities.
     */
    VulkanInstance();

    /**
     * @brief Destructor for the VulkanInstance class.
     *
     * This destructor cleans up the Vulkan instance by destroying the debug messenger
     * utilities and other related resources.
     *
     * It ensures that the resources allocated during the lifespan of the VulkanInstance
     * are properly released.
     */
    ~VulkanInstance() override;

    VkInstance Get() override { return _instance; }

  private:
    /**
     * @brief Creates a debug utils messenger.
     *
     * This method sets up debugging utilities by creating a messenger to capture and handle
     * validation messages. It uses the `vkGetInstanceProcAddr` to fetch the address of the
     * `vkCreateDebugUtilsMessengerEXT` function and calls it to initialize the messenger.
     *
     * @param instance The Vulkan instance used to create the debug messenger.
     * @param pCreateInfo Pointer to a `VkDebugUtilsMessengerCreateInfoEXT` structure containing the
     *                    initialization parameters for the debug messenger.
     * @param pAllocator Optional pointer to a `VkAllocationCallbacks` structure to customize memory allocation.
     * @param pDebugMessenger Pointer to a `VkDebugUtilsMessengerEXT` handle in which the created debug messenger is returned.
     * @return `VK_SUCCESS` on successful creation of the debug messenger,
     *         or `VK_ERROR_EXTENSION_NOT_PRESENT` if the extension is not available.
     */
    VkResult CreateDebugUtilsMessengerEXT(
      VkInstance instance,
      const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
      const VkAllocationCallbacks *pAllocator,
      VkDebugUtilsMessengerEXT *pDebugMessenger);

    void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                       VkDebugUtilsMessengerEXT debugMessenger,
                                       const VkAllocationCallbacks *pAllocator);

    static VKAPI_ATTR VkBool32 VKAPI_CALL
    debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                  VkDebugUtilsMessageTypeFlagsEXT messageType,
                  const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                  void *pUserData) {
      if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
        spdlog::error(pCallbackData->pMessage);

      if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        spdlog::warn(pCallbackData->pMessage);

      if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
        spdlog::info(pCallbackData->pMessage);

      if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
        spdlog::info(pCallbackData->pMessage);

      return VK_FALSE;
    }

    std::vector<const char *> _validationLayers = {"VK_LAYER_KHRONOS_validation"};

    VkInstance _instance = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT _debugMessenger = VK_NULL_HANDLE;
  };
} // namespace Entropy::Graphics::Vulkan::Instances

#endif // ENTROPY_VULKAN_INSTANCE_H
