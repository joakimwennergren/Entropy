#include "vk_instance.hpp"

using namespace Entropy::Graphics::Vulkan::Instances;

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
  for (const auto &extension: extensionProps) {
    extensions.push_back(extension.extensionName);
  }

  extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);

#ifdef BUILD_FOR_MACOS
  extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
  extensions.push_back("VK_MVK_macos_surface");
#endif

#ifdef BUILD_FOR_ANDROID
  extensions.push_back("VK_KHR_surface");
  extensions.push_back("VK_KHR_android_surface");
  // extensions.push_back("VK_KHR_surface_protected_capabilities");
#endif

  createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  createInfo.ppEnabledExtensionNames = extensions.data();

  if (ValidationLayers::ValidationLayer::CheckValidationLayerSupport(_validationLayers)) {
    createInfo.enabledLayerCount = static_cast<uint32_t>(_validationLayers.size());
    createInfo.ppEnabledLayerNames = _validationLayers.data();
  }

  vkCreateInstance(&createInfo, nullptr, &_instance);

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
  dbgCreateInfo.pUserData = nullptr;

  VK_CHECK(CreateDebugUtilsMessengerEXT(_instance, &dbgCreateInfo, nullptr,
    &_debugMessenger));
}

VulkanInstance::~VulkanInstance() {
  DestroyDebugUtilsMessengerEXT(_instance, _debugMessenger, nullptr);
  vkDestroyInstance(_instance, nullptr);
}

VkResult VulkanInstance::CreateDebugUtilsMessengerEXT(
  const VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
  const VkAllocationCallbacks *pAllocator,
  VkDebugUtilsMessengerEXT *pDebugMessenger) {
  const auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(
    instance, "vkCreateDebugUtilsMessengerEXT"));
  if (func != nullptr) {
    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
  }
  return VK_ERROR_UNKNOWN;
}

void VulkanInstance::DestroyDebugUtilsMessengerEXT(
  const VkInstance instance, const VkDebugUtilsMessengerEXT debugMessenger,
  const VkAllocationCallbacks *pAllocator) {
  const auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(
    instance, "vkDestroyDebugUtilsMessengerEXT"));
  if (func != nullptr) {
    func(instance, debugMessenger, pAllocator);
  }
}
