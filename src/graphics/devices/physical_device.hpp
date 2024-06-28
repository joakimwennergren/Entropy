#pragma once

#include <config.hpp>
#include <iostream>
#include <set>
#include <string>
#include <vulkan/vulkan.hpp>

#include "spdlog/spdlog.h"

#include <graphics/instances/vk_instance.hpp>
#include <graphics/queuefamilies/queuefamily.hpp>
#include <graphics/surfaces/surface.hpp>
#include <graphics/swapchains/swapchain.hpp>
#include <services/service.hpp>

using namespace Entropy::Graphics::Instances;
using namespace Entropy::Graphics::Surfaces;
using namespace Entropy::Graphics::QueueFamilies;
using namespace Entropy::Graphics::Swapchains;

using namespace Entropy::Services;

namespace Entropy {
namespace Graphics {
namespace Devices {
class PhysicalDevice : public Service {
public:
  PhysicalDevice() = default;
  PhysicalDevice(std::shared_ptr<VulkanInstance> instance,
                 std::shared_ptr<WindowSurface> surface);
  PhysicalDevice(std::shared_ptr<VulkanInstance> instance);
  inline VkPhysicalDevice Get() { return _physicalDevice; };

#ifdef BUILD_FOR_MACOS
  const std::vector<const char *> deviceExtensions = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME, "VK_KHR_portability_subset"};
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
};
} // namespace Devices
} // namespace Graphics
} // namespace Entropy
