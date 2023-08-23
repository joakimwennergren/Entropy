/**
 * @file buffer.hpp
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
#include "context.hpp"

using namespace Symbios::Core;

namespace Symbios
{
    namespace Graphics
    {
        namespace Buffers
        {
            /**
             * @brief
             *
             */
            class Buffer
            {
            public:
                Buffer(std::shared_ptr<Context> context, uint32_t size);
                ~Buffer();

            private:
                uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

                std::shared_ptr<Context> _context;
                VkBuffer _buffer;
                VkDeviceMemory _bufferMemory;
            };
        }
    }
}
