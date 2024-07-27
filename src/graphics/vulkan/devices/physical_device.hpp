#pragma once

#include "iphysical_device.hpp"
#include <config.hpp>
#include <graphics/vulkan/instances/vk_instance.hpp>
#include <spdlog/spdlog.h>
#include <vulkan/vulkan.hpp>

using namespace Entropy::Graphics::Vulkan::Instances;

namespace Entropy
{
  namespace Graphics
  {
    namespace Vulkan
    {
      namespace Devices
      {
        /**
         * @brief Wrapper for vulkan physical device
         * @author Joakim Wennergren
         * @since Tue Jul 02 2024
         */
        struct PhysicalDevice : public ServiceBase<IPhysicalDevice>
        {

          /**
           * @brief Construct a new PhysicalDevice (simplified)
           * @param instance VulkanInstance
           */
          PhysicalDevice()
          {
            ServiceLocator *sl = ServiceLocator::GetInstance();
            auto vulkanInstance = sl->getService<IVulkanInstance>();

            uint32_t deviceCount;
            std::vector<VkPhysicalDevice> physicalDevices;

            vkEnumeratePhysicalDevices(vulkanInstance->Get(), &deviceCount, NULL);
            physicalDevices.resize(deviceCount);

            vkEnumeratePhysicalDevices(vulkanInstance->Get(), &deviceCount,
                                       physicalDevices.data());

            assert(physicalDevices.size() != 0);

            _physicalDevice = physicalDevices[0];
          }

          inline VkPhysicalDevice Get() override { return _physicalDevice; };

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
        };
      } // namespace Devices
    } // namespace Vulkan
  } // namespace Graphics
} // namespace Entropy
