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

#include <global/vulkancontext.hpp>

using namespace Entropy::Global;

namespace Entropy
{
    namespace Graphics
    {
        namespace Utilities
        {
            /**
             * @brief Utlity class for graphic specific tasks
             *
             */
            class Utility
            {
            public:
                /**
                 * @brief Find index of a physical device memory property
                 *
                 * @param _context Vulkan context
                 * @param typeFilter ??
                 * @param properties Properties to match against
                 * @return uint32_t Index of matched property
                 */
                static uint32_t FindMemoryTypeIndex(uint32_t typeFilter, VkMemoryPropertyFlags properties);

                /**
                 * @brief Wrapper functions for aligned memory allocation
                 * There is currently no standard for this in C++ that works across all platforms and vendors, so we abstract this
                 * @param size
                 * @param alignment
                 * @return void*
                 */

                static void *AlignedAlloc(size_t size, size_t alignment);

                /**
                 * @brief
                 *
                 * @param data
                 */
                void AlignedFree(void *data);
            };
        }
    }
}
