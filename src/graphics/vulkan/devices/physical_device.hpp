#pragma once

#include <config.hpp>
#include <iostream>
#include <set>
#include <string>
#include <vulkan/vulkan.hpp>

#include "spdlog/spdlog.h"

#include <graphics/vulkan/instances/vk_instance.hpp>
#include <graphics/vulkan/queuefamilies/queuefamily.hpp>
#include <graphics/vulkan/surfaces/surface.hpp>

using namespace Entropy::Graphics::Vulkan::Instances;
using namespace Entropy::Graphics::Vulkan::Surfaces;
using namespace Entropy::Graphics::Vulkan::QueueFamilies;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Devices {
class PhysicalDevice {
public:
  /*
    PhysicalDevice() = default;

    PhysicalDevice(std::shared_ptr<VulkanInstance> instance,
                   std::shared_ptr<WindowSurface> surface);

  */
  PhysicalDevice(VulkanInstance instance) : _instance{instance} {

    uint32_t deviceCount;
    std::vector<VkPhysicalDevice> physicalDevices;

    vkEnumeratePhysicalDevices(instance.Get(), &deviceCount, NULL);
    physicalDevices.resize(deviceCount);

    vkEnumeratePhysicalDevices(instance.Get(), &deviceCount,
                               physicalDevices.data());

    _physicalDevice = physicalDevices[0];
  }

  inline VkPhysicalDevice Get() { return _physicalDevice; };

#ifdef BUILD_FOR_MACOS
  std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME,
                                                "VK_KHR_portability_subset"};
#endif

#ifdef BUILD_FOR_IOS
  const std::vector<const char *> deviceExtensions = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME, "VK_KHR_portability_subset"};
#endif

#ifdef BUILD_FOR_WINDOWS
  const std::vector<const char *> deviceExtensions = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME};
#endif

#ifdef BUILD_FOR_ANDROID
  const std::vector<const char *> deviceExtensions = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME};
#endif

private:
  bool IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface,
                        const std::vector<const char *> deviceExtensions);
  bool CheckDeviceExtensionSupport(VkPhysicalDevice device,
                                   const std::vector<const char *> extensions);
  VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
  VulkanInstance _instance;
};
} // namespace Devices
} // namespace Graphics
} // namespace Entropy
}
