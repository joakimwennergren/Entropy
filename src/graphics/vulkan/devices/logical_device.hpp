#pragma once

#include <graphics/vulkan/devices/physical_device.hpp>
#include <vulkan/vulkan.hpp>

using namespace Entropy::Graphics::Vulkan::Devices;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Devices {

/**
 * @brief Wrapper for vulkan logical device
 * @author Joakim Wennergren
 * @since Tue Jul 02 2024
 */
struct LogicalDevice {
  /**
   * @brief Constructor for a logical device
   * @param physicalDevice PhysicalDevice dependency
   */
  LogicalDevice(PhysicalDevice phyiscalDevice)
      : _physicalDevice{phyiscalDevice} {

    uint32_t idx = 0;
    uint32_t queueFamilyPropertyCount;
    std::vector<VkQueueFamilyProperties> queueFamilyProperties;

    vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice.Get(),
                                             &queueFamilyPropertyCount, NULL);

    queueFamilyProperties.resize(queueFamilyPropertyCount);

    vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice.Get(),
                                             &queueFamilyPropertyCount,
                                             queueFamilyProperties.data());

    for (int i = 0; i < queueFamilyPropertyCount; i++) {
      if (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
        queueFamiliy = idx;
        break;
      }
      ++idx;
    }

    float queuePriority = 1.0f;

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueFamiliy;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_TRUE;
    deviceFeatures.fillModeNonSolid = VK_TRUE;

    // Enable all features: just pass the physical features 2 struct.
    VkPhysicalDeviceDescriptorIndexingFeaturesEXT indexingFeatures = {};
    indexingFeatures.sType =
        VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES_EXT;
    indexingFeatures.runtimeDescriptorArray = VK_TRUE;
    indexingFeatures.shaderSampledImageArrayNonUniformIndexing = VK_TRUE;
    indexingFeatures.descriptorBindingPartiallyBound = VK_TRUE;
    indexingFeatures.shaderSampledImageArrayNonUniformIndexing = VK_TRUE;

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount =
        static_cast<uint32_t>(_physicalDevice.deviceExtensions.size());
    createInfo.ppEnabledExtensionNames =
        _physicalDevice.deviceExtensions.data();
    createInfo.pNext = &indexingFeatures;

    vkCreateDevice(_physicalDevice.Get(), &createInfo, nullptr,
                   &_logicalDevice);

    vkGetDeviceQueue(_logicalDevice, queueFamiliy, 0, &_graphicsQueue);
  }

  /**
   * @brief Get the logical device handle
   * @return VkDevice
   */
  inline VkDevice Get() { return _logicalDevice; };

  /**
   * @brief Get the graphicsqueue
   * @return VkQueue
   */
  inline VkQueue GetGraphicQueue() { return _graphicsQueue; };

  // QueueFamily index
  uint32_t queueFamiliy = 0;

private:
  // Logical device handle
  VkDevice _logicalDevice = VK_NULL_HANDLE;
  // GraphicsQueue handle
  VkQueue _graphicsQueue = VK_NULL_HANDLE;
  // Dependecies
  PhysicalDevice _physicalDevice;
};
} // namespace Devices
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy