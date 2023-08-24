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
                /**
                 * @brief Construct a new Buffer object
                 *
                 */
                Buffer() = default;

                /**
                 * @brief Construct a new Buffer object
                 *
                 * @param context
                 */
                Buffer(std::shared_ptr<Context> context);

                /**
                 * @brief Destroy the Buffer object
                 *
                 */
                ~Buffer();

                /**
                 * @brief Create a Vertex Buffer object
                 *
                 */
                void CreateVertexBuffer(std::vector<Vertex> vertices);

                /**
                 * @brief Create a Index Buffer object
                 *
                 */
                void CreateIndexBufferUint16(std::vector<uint16_t> indices);

                /**
                 * @brief
                 *
                 * @param srcBuffer
                 * @param dstBuffer
                 * @param size
                 */
                void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

                /**
                 * @brief Get the Buffer object
                 *
                 * @return VkBuffer
                 */
                inline VkBuffer GetBuffer() { return this->_buffer; };

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
                 * @brief
                 *
                 * @param typeFilter
                 * @param properties
                 * @return uint32_t
                 */

                static uint32_t FindMemoryType(std::shared_ptr<Context> _context, uint32_t typeFilter, VkMemoryPropertyFlags properties);

            private:
                std::shared_ptr<Context> _context;
                VkBuffer _buffer;
                VkDeviceMemory _bufferMemory;
            };
        }
    }
}
