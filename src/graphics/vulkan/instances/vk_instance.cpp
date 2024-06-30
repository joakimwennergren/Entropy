#include "vk_instance.hpp"

using namespace Entropy::Graphics::Vulkan::Instances;

/*
VulkanInstance::VulkanInstance() {
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Entropy Application";
  appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
  appInfo.pEngineName = "Entropy";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_3;

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

#ifdef BUILD_FOR_MACOS
  createInfo.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

  uint32_t extensionCount = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

  std::vector<VkExtensionProperties> extensionProps(extensionCount);
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,
                                         extensionProps.data());

  std::vector<const char *> extensions;
  for (const auto &extension : extensionProps) {
    extensions.push_back(extension.extensionName);
  }

#ifdef BUILD_FOR_MACOS
  extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
#endif

#ifdef BUILD_FOR_ANDROID
  extensions.push_back("VK_KHR_surface");
  extensions.push_back("VK_KHR_android_surface");
  // extensions.push_back("VK_KHR_surface_protected_capabilities");
#endif

  createInfo.enabledExtensionCount = (uint32_t)extensions.size();
  createInfo.ppEnabledExtensionNames = extensions.data();

  if (ValidationLayer::CheckValidationLayerSupport(_validationLayers)) {
    createInfo.enabledLayerCount = (uint32_t)_validationLayers.size();
    createInfo.ppEnabledLayerNames = _validationLayers.data();
  }

  if (vkCreateInstance(&createInfo, nullptr, &this->_instance) != VK_SUCCESS) {
    std::cout << "Failed to create vulkan instance!" << std::endl;
    exit(EXIT_FAILURE);
  }

  VkDebugUtilsMessengerCreateInfoEXT dbgCreateInfo{};
  dbgCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  dbgCreateInfo.messageSeverity =
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  dbgCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                              VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                              VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  dbgCreateInfo.pfnUserCallback = debugCallback;
  dbgCreateInfo.pUserData = nullptr; // Optional

  if (CreateDebugUtilsMessengerEXT(_instance, &dbgCreateInfo, nullptr,
                                   &_debugMessenger) != VK_SUCCESS) {
    throw std::runtime_error("failed to set up debug messenger!");
  }
}

VulkanInstance::~VulkanInstance() {
  DestroyDebugUtilsMessengerEXT(_instance, _debugMessenger, nullptr);
}

VkResult VulkanInstance::CreateDebugUtilsMessengerEXT(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator,
    VkDebugUtilsMessengerEXT *pDebugMessenger) {
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr) {
    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void VulkanInstance::DestroyDebugUtilsMessengerEXT(
    VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks *pAllocator) {
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkDestroyDebugUtilsMessengerEXT");
  if (func != nullptr) {
    func(instance, debugMessenger, pAllocator);
  }
}
*/