#pragma once

#include "iphysical_device.hpp"
#include <graphics/vulkan/instances/vk_instance.hpp>
#include <vulkan/vulkan.hpp>

using namespace Entropy::Graphics::Vulkan::Instances;

namespace Entropy::Graphics::Vulkan::Devices {
    /**
     * @brief Represents a physical device in Vulkan, serving as an interface for querying device properties and capabilities.
     *
     * The PhysicalDevice class provides functions for device enumeration, querying physical device properties,
     * and determining the capabilities of the physical device. It plays a pivotal role in the Vulkan setup,
     * facilitating the communication between the application and the GPU hardware.
     */
    struct PhysicalDevice final : ServiceBase<IPhysicalDevice> {
        /**
         * @brief Constructs the PhysicalDevice object and initializes the Vulkan physical device.
         * This method retrieves the Vulkan instance from the service locator, enumerates
         * the available physical devices, and selects the first physical device.
         */
        PhysicalDevice() {
            const ServiceLocator *sl = ServiceLocator::GetInstance();
            const auto vulkanInstance = sl->getService<IVulkanInstance>();

            uint32_t deviceCount;
            std::vector<VkPhysicalDevice> physicalDevices;

            vkEnumeratePhysicalDevices(vulkanInstance->Get(), &deviceCount, nullptr);
            physicalDevices.resize(deviceCount);

            vkEnumeratePhysicalDevices(vulkanInstance->Get(), &deviceCount,
                                       physicalDevices.data());

            assert(!physicalDevices.empty());

            _physicalDevice = physicalDevices[0];
        }

        VkPhysicalDevice Get() override { return _physicalDevice; }

    private:
        /**
         * @brief Checks if a given Vulkan physical device is suitable for rendering.
         *
         * This method evaluates if the specified Vulkan physical device meets the
         * necessary criteria for rendering by checking for required device extensions
         * and surface capabilities.
         *
         * @param device The Vulkan physical device to be evaluated.
         * @param surface The Vulkan surface for presentation support.
         * @param deviceExtensions List of required device extensions.
         *
         * @return Returns true if the device is suitable for rendering, otherwise returns false.
         */
        bool IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface,
                              std::vector<const char *> deviceExtensions);

        /**
         * @brief Checks if the specified Vulkan physical device supports the required extensions.
         *
         * This method iterates through the list of required extensions and verifies if all of them
         * are supported by the Vulkan physical device provided.
         *
         * @param device The Vulkan physical device to be checked.
         * @param extensions A list of required extension names to check for support.
         *
         * @return Returns true if all specified extensions are supported by the device; otherwise, returns false.
         */
        bool CheckDeviceExtensionSupport(VkPhysicalDevice device,
                                         std::vector<const char *> extensions);

        // physical device handle
        VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
    };
} // namespace Entropy::Graphics::Vulkan::Devices
