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

#include "context.hpp"

using namespace Symbios::Core;

namespace Symbios
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
                static uint32_t FindMemoryTypeIndex(std::shared_ptr<Context> _context, uint32_t typeFilter, VkMemoryPropertyFlags properties);
            };
        }
    }
}