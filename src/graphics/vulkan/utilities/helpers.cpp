#include "helpers.hpp"

/**
 * @brief Finds a compatible memory type index for a Vulkan physical device.
 *
 * This function iterates over the memory types of a Vulkan physical device
 * and determines the index of the first memory type that satisfies both
 * the type filter and the specified memory property flags.
 *
 * @param physicalDevice The Vulkan physical device to query for memory properties.
 * @param typeFilter A bitmask specifying the memory type indices that are
 *                   suitable for the required operation.
 * @param properties Vulkan memory property flags specifying additional required
 *                   properties like host-visible or device-local memory.
 * @return The index of the first memory type that matches the specified criteria.
 *         Returns 0 if no suitable memory type is found. Note that 0 might also
 *         represent a valid memory type index; proper input validation is assumed.
 */
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