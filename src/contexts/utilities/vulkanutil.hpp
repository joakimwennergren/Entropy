/**
 * @file utilities.hpp
 * @author Joakim Wennergren (joakim.wennergren@databeams.se)
 * @brief
 * @version 0.1
 * @date 2023-08-22
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include <vulkan/vulkan.hpp>

namespace Entropy
{
    namespace Contexts
    {
        namespace VulkanUtilities
        {
            class VulkanUtility
            {
            public:
                static bool CheckDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char *> extensions);
            };
        }
    }
}
