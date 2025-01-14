#pragma once

#include <graphics/vulkan/devices/iphysical_device.hpp>
#include <spdlog/spdlog.h>
#include <vulkan/vk_enum_string_helper.h>

#define VK_CHECK(x)                                                            \
do {                                                                           \
    VkResult err = x;                                                          \
    if (err) {                                                                 \
      spdlog::error(__FILE__);                                                 \
      spdlog::error(__LINE__);                                                 \
      spdlog::error(string_VkResult(err));                                     \
      abort();                                                                 \
    }                                                                          \
} while (0)

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
                             uint32_t typeFilter,
                             VkMemoryPropertyFlags properties);

/**
 * @brief Retrieves the color format to be used for rendering or image operations.
 *
 * This function returns the Vulkan format that represents the color format
 * selected for the current context or operation.
 *
 * @return A VkFormat value representing the color format.
 *         The specific value depends on the configuration or system settings.
 */
VkFormat GetColorFormat();
