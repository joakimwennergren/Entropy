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
#include "vertex.hpp"
#include "utilities.hpp"
#include "commandbuffer.hpp"
#include "vulkancontext.hpp"

using namespace Symbios::Core;
using namespace Symbios::Graphics::Utilities;
using namespace Symbios::Graphics::CommandBuffers;

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

                /**
                 * @brief Construct a new Buffer object
                 *
                 * @param context Vulkan context
                 */
                Buffer();

                /**
                 * @brief Destroy the Buffer object
                 *
                 */
                ~Buffer();

                /**
                 * @brief Create a Index Buffer object
                 *
                 */
                void CreateIndexBufferUint16(std::vector<uint16_t> indices);

                /**
                 * @brief Get the vulkan buffer
                 *
                 * @return VkBuffer
                 */
                inline VkBuffer GetVulkanBuffer() { return this->_buffer; };

                /**
                 * @brief Get the Buffer Memory object
                 *
                 * @return VkDeviceMemory
                 */
                inline VkDeviceMemory GetBufferMemory() { return this->_bufferMemory; };

                /**
                 * @brief Get the Mapped Memory object
                 *
                 * @return void*
                 */
                inline void *GetMappedMemory() { return this->_mappedMemory; };

            protected:
                /**
                 * @brief Create a Buffer object
                 *
                 * @param size
                 * @param usage
                 * @param properties
                 * @param buffer
                 * @param bufferMemory
                 */
                static void CreateBuffer(std::shared_ptr<Context> _context, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory);

                /**
                 * @brief Copy One Buffer into another
                 *
                 * @param srcBuffer VkBuffer source
                 * @param dstBuffer VkBuffer destination
                 * @param size srcBuffer size
                 */
                void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

                // Vulkan context
                std::shared_ptr<Context> _context;

                // Vulkan buffer
                VkBuffer _buffer;

                // Buffer in device memory
                VkDeviceMemory _bufferMemory;

                // Mapped memory towards GPU?
                void *_mappedMemory;
            };
        }
    }
}
