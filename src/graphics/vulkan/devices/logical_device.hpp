#pragma once

#include "ilogical_device.hpp"
#include "iphysical_device.hpp"
#include <vulkan/vulkan.hpp>

namespace Entropy::Graphics::Vulkan::Devices {
  /**
   * @brief A class that manages the Vulkan logical device and associated graphics queue.
   *
   * This class is responsible for initializing and configuring a Vulkan logical device
   * with the necessary features, queue information, and extensions. It provides methods
   * to retrieve the logical device handle and the graphics queue handle.
   */
  struct LogicalDevice final : ServiceBase<ILogicalDevice> {
    /**
     * @brief Initializes the logical device with necessary features and queue information.
     *
     * This constructor sets up a Vulkan logical device by querying physical device properties,
     * selecting a suitable queue family for graphics operations, and enabling required device features.
     * It also sets up the necessary Vulkan structures and creates the logical device along with the graphics queue.
     */
    LogicalDevice() {
      const ServiceLocator *sl = ServiceLocator::GetInstance();
      const auto physicalDevice = sl->getService<IPhysicalDevice>();

      uint32_t idx = 0;
      uint32_t queueFamilyPropertyCount;
      std::vector<VkQueueFamilyProperties> queueFamilyProperties;

      vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice->Get(),
                                               &queueFamilyPropertyCount, nullptr);

      queueFamilyProperties.resize(queueFamilyPropertyCount);

      vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice->Get(),
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

      VkPhysicalDeviceDescriptorIndexingFeatures indexingFeatures = {};
      indexingFeatures.sType =
          VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES;
      //indexingFeatures.descriptorBindingUpdateUnusedWhilePending = VK_TRUE;
      // indexingFeatures.runtimeDescriptorArray = VK_TRUE;
      // indexingFeatures.shaderSampledImageArrayNonUniformIndexing = VK_TRUE;
      indexingFeatures.descriptorBindingPartiallyBound = VK_TRUE;
      // indexingFeatures.shaderSampledImageArrayNonUniformIndexing = VK_TRUE;

      VkDeviceCreateInfo createInfo{};
      createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
      createInfo.queueCreateInfoCount = 1;
      createInfo.pQueueCreateInfos = &queueCreateInfo;
      createInfo.pEnabledFeatures = &deviceFeatures;
      createInfo.enabledExtensionCount =
          static_cast<uint32_t>(physicalDevice->deviceExtensions.size());
      createInfo.ppEnabledExtensionNames =
          physicalDevice->deviceExtensions.data();
      createInfo.pNext = &indexingFeatures;

      vkCreateDevice(physicalDevice->Get(), &createInfo, nullptr,
                     &_logicalDevice);

      vkGetDeviceQueue(_logicalDevice, queueFamiliy, 0, &_graphicsQueue);
    }

    /**
     * @brief Returns the Vulkan logical device handle.
     *
     * This method provides access to the Vulkan logical device handle initialized and configured by this class.
     *
     * @return The handle to the Vulkan logical device.
     */
    VkDevice Get() override { return _logicalDevice; }

    /**
     * @brief Retrieves the graphics queue handle from the logical device.
     *
     * This method returns the handle to the graphics queue, which was initialized during the logical device setup.
     *
     * @return The handle to the Vulkan graphics queue.
     */
    VkQueue GetGraphicQueue() override { return _graphicsQueue; }

  private:
    // Logical device handle
    VkDevice _logicalDevice = VK_NULL_HANDLE;
    // GraphicsQueue handle
    VkQueue _graphicsQueue = VK_NULL_HANDLE;
  };
} // namespace Entropy::Graphics::Vulkan::Devices
