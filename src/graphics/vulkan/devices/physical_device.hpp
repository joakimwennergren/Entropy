#pragma once

#include <config.hpp>
#include <graphics/vulkan/instances/vk_instance.hpp>
#include <spdlog/spdlog.h>
#include <vulkan/vulkan.hpp>

using namespace Entropy::Graphics::Vulkan::Instances;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Devices {
/**
 * @brief Wrapper for vulkan physical device
 * @author Joakim Wennergren
 * @since Tue Jul 02 2024
 */
struct PhysicalDevice {

  /**
   * @brief Construct a new PhysicalDevice (simplified)
   * @param instance VulkanInstance
   */
  PhysicalDevice(VulkanInstance instance) : _instance{instance} {

    uint32_t deviceCount;
    std::vector<VkPhysicalDevice> physicalDevices;

    vkEnumeratePhysicalDevices(instance.Get(), &deviceCount, NULL);
    physicalDevices.resize(deviceCount);

    vkEnumeratePhysicalDevices(instance.Get(), &deviceCount,
                               physicalDevices.data());

    assert(physicalDevices.size() != 0);

    _physicalDevice = physicalDevices[0];
  }

  inline VkPhysicalDevice Get() { return _physicalDevice; };

#ifdef BUILD_FOR_MACOS
  std::vector<const char *> deviceExtensions = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME,
      VK_KHR_SHADER_DRAW_PARAMETERS_EXTENSION_NAME,
      VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME, "VK_EXT_descriptor_indexing",
      "VK_KHR_portability_subset"};
#endif

#ifdef BUILD_FOR_IOS
  std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME,
                                                "VK_KHR_portability_subset"};
#endif

#ifdef BUILD_FOR_WINDOWS
  std::vector<const char *> deviceExtensions = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME};
#endif

#ifdef BUILD_FOR_ANDROID
  std::vector<const char *> deviceExtensions = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME};
#endif

private:
  /**
   * @brief Helper method to see if device is suitable for vulkan
   * @param device
   * @param surface
   * @param deviceExtensions
   * @return
   */
  bool IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface,
                        const std::vector<const char *> deviceExtensions);

  /**
   * @brief Helper method to check device extension support
   * @param device
   * @param extensions Extensions to check
   * @return Bool supported extensions?
   */
  bool CheckDeviceExtensionSupport(VkPhysicalDevice device,
                                   const std::vector<const char *> extensions);

  // physical device handle
  VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;

  // Depedencies
  VulkanInstance _instance;
};
} // namespace Devices
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy
