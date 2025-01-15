#include "helpers.hpp"

uint32_t FindMemoryTypeIndex(const std::shared_ptr<IPhysicalDevice> &physicalDevice,
                             const uint32_t typeFilter,
                             const VkMemoryPropertyFlags properties) {
    // Get the physical device's memory properties
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice->Get(), &memProperties);

    // Iterate over memory properties and return index of matched property
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) &&
            (memProperties.memoryTypes[i].propertyFlags & properties) ==
            properties) {
            return i;
        }
    }
    return 0;
}

VkFormat GetColorFormat() {
#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_LINUX)
    return VK_FORMAT_B8G8R8A8_UNORM;
#elif defined(BUILD_FOR_WINDOWS)
    return VK_FORMAT_R8G8B8A8_UNORM;
#else
    return VK_FORMAT_B8G8R8A8_SRGB;
#endif
}
