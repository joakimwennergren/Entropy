#pragma once
#include "vulkan/vulkan_core.h"
#include <cstdlib>
#include <iostream>
#include <vulkan/vulkan.hpp>

#include <spdlog/spdlog.h>

#include <config.hpp>
#include <graphics/vulkan/validationlayers/validationlayer.hpp>

using namespace Entropy::Graphics::Vulkan::ValidationLayers;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Instances {

struct VulkanInstance {
public:
  VulkanInstance();
  ~VulkanInstance();
  inline VkInstance Get() { return _instance; };

private:
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

} // namespace Instances
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy
