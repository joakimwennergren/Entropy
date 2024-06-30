#pragma once

#include "vulkan/vulkan_core.h"
#include <config.hpp>
#include <iostream>
#include <set>
#include <string>
#include <vulkan/vulkan.hpp>

#include <graphics/devices/physical_device.hpp>
#include <graphics/vulkan/queuefamilies/queuefamily.hpp>
#include <graphics/vulkan/surfaces/surface.hpp>
#include <services/service.hpp>

using namespace Entropy::Graphics::Vulkan::Surfaces;
using namespace Entropy::Graphics::Vulkan::QueueFamilies;
using namespace Entropy::Graphics::Devices;

namespace Entropy {
namespace Graphics {
namespace Devices {

class LogicalDevice : public Service {
public:
  /*
  LogicalDevice() = default;
  LogicalDevice(std::shared_ptr<PhysicalDevice> physicalDevice,
                std::shared_ptr<WindowSurface> surface);
  LogicalDevice(std::shared_ptr<PhysicalDevice> physicalDevice);
  */

  LogicalDevice(PhysicalDevice pd) : _physicalDevice{pd} {

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

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount =
        static_cast<uint32_t>(_physicalDevice.deviceExtensions.size());
    createInfo.ppEnabledExtensionNames =
        _physicalDevice.deviceExtensions.data();

    vkCreateDevice(_physicalDevice.Get(), &createInfo, nullptr,
                   &_logicalDevice);

    vkGetDeviceQueue(_logicalDevice, queueFamiliy, 0, &_graphicsQueue);
  }

  inline VkDevice Get() { return _logicalDevice; };
  inline VkQueue GetGraphicQueue() { return _graphicsQueue; };
  inline VkQueue GetPresentQueue() { return _presentQueue; };

  uint32_t queueFamiliy = 0;

private:
  VkDevice _logicalDevice = VK_NULL_HANDLE;
  VkQueue _graphicsQueue = VK_NULL_HANDLE;
  VkQueue _presentQueue = VK_NULL_HANDLE;
  PhysicalDevice _physicalDevice;
};
} // namespace Devices
} // namespace Graphics
} // namespace Entropy
