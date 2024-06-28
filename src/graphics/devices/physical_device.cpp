#include "physical_device.hpp"

using namespace Entropy::Graphics::Devices;

PhysicalDevice::PhysicalDevice(std::shared_ptr<VulkanInstance> instance,
                               std::shared_ptr<WindowSurface> surface) {
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(instance->Get(), &deviceCount, nullptr);

  if (deviceCount == 0) {
    spdlog::error("Enumerate physical devices returned {} devices",
                  deviceCount);
    return;
  }

  std::vector<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(instance->Get(), &deviceCount, devices.data());

  for (const auto &device : devices) {
    if (IsDeviceSuitable(device, surface->Get(), deviceExtensions)) {
      _physicalDevice = device;
      break;
    }
  }

  if (_physicalDevice == VK_NULL_HANDLE) {
    spdlog::error("Couldn't find a suitable physical device regarding device "
                  "extension and swapchain support");
  }
}

PhysicalDevice::PhysicalDevice(std::shared_ptr<VulkanInstance> instance) {

  uint32_t deviceCount;
  std::vector<VkPhysicalDevice> physicalDevices;

  vkEnumeratePhysicalDevices(instance->Get(), &deviceCount, NULL);

  physicalDevices.resize(deviceCount);

  vkEnumeratePhysicalDevices(instance->Get(), &deviceCount,
                             physicalDevices.data());

  _physicalDevice = physicalDevices[0];
}

bool PhysicalDevice::IsDeviceSuitable(
    VkPhysicalDevice device, VkSurfaceKHR surface,
    const std::vector<const char *> deviceExtensions) {
  QueueFamilyIndices indices = QueueFamily::FindQueueFamilies(device, surface);

  bool extensionsSupported =
      CheckDeviceExtensionSupport(device, deviceExtensions);

  bool swapChainAdequate = false;
  if (extensionsSupported) {
    SwapChainSupportDetails swapChainSupport =
        Swapchain::QuerySwapChainSupport(device, surface);
    swapChainAdequate = !swapChainSupport.formats.empty() &&
                        !swapChainSupport.presentModes.empty();
  }

  return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

bool PhysicalDevice::CheckDeviceExtensionSupport(
    VkPhysicalDevice device, const std::vector<const char *> extensions) {
  uint32_t extensionCount;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       nullptr);

  std::vector<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       availableExtensions.data());

  std::set<std::string> requiredExtensions(extensions.begin(),
                                           extensions.end());

  for (const auto &extension : availableExtensions) {
    requiredExtensions.erase(extension.extensionName);
  }

  return requiredExtensions.empty();
}