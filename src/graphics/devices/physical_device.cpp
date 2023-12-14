#include "physical_device.hpp"

using namespace Entropy::Graphics::Devices;

PhysicalDevice::PhysicalDevice(std::shared_ptr<VulkanInstance> instance, std::shared_ptr<WindowSurface> surface)
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance->Get(), &deviceCount, nullptr);

    if (deviceCount == 0)
    {
        exit(EXIT_FAILURE);
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance->Get(), &deviceCount, devices.data());

    for (const auto &device : devices)
    {
        if (IsDeviceSuitable(device, surface->Get(), deviceExtensions))
        {
            _physicalDevice = device;
            break;
        }
    }

    if (_physicalDevice == VK_NULL_HANDLE)
    {
        exit(EXIT_FAILURE);
    }
}

bool PhysicalDevice::IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, const std::vector<const char *> deviceExtensions)
{
    QueueFamilyIndices indices = QueueFamily::FindQueueFamilies(device, surface);

    bool extensionsSupported = CheckDeviceExtensionSupport(device, deviceExtensions);

    bool swapChainAdequate = false;
    if (extensionsSupported)
    {
        SwapChainSupportDetails swapChainSupport = Swapchain::QuerySwapChainSupport(device, surface);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

bool PhysicalDevice::CheckDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char *> extensions)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(extensions.begin(), extensions.end());

    for (const auto &extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}