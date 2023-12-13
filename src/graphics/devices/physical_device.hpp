#pragma once

#include <vulkan/vulkan.hpp>
#include <iostream>
#include <set>
#include <string>
#include <config.hpp>

#include <graphics/instances/vk_instance.hpp>
#include <graphics/surfaces/surface.hpp>
#include <graphics/queuefamilies/queuefamily.hpp>
#include <graphics/swapchains/swapchain.hpp>

using namespace Entropy::Graphics::Instances;
using namespace Entropy::Graphics::Surfaces;
using namespace Entropy::Graphics::QueueFamilies;
using namespace Entropy::Graphics::Swapchains;

namespace Entropy
{
    namespace Graphics
    {
        namespace Devices
        {
            class PhysicalDevice
            {
            public:
                PhysicalDevice(std::shared_ptr<VulkanInstance> instance, WindowSurface surface);
                inline VkPhysicalDevice Get() { return _physicalDevice; };
#ifdef BUILD_FOR_MACOS
                const std::vector<const char *> deviceExtensions = {
                    VK_KHR_SWAPCHAIN_EXTENSION_NAME, "VK_KHR_portability_subset"};
#endif

#ifdef BUILD_FOR_ANDROID
                const std::vector<const char *> deviceExtensions = {
                    "VK_KHR_swapchain"};
#endif
            private:
                bool IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, const std::vector<const char *> deviceExtensions);
                bool CheckDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char *> extensions);
                VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
            };
        }
    }
}
