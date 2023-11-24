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
#include <graphics/data/vertex.hpp>
#include <graphics/utilities/utilities.hpp>
#include <graphics/commandbuffers/commandbuffer.hpp>
#include <global/vulkancontext.hpp>

using namespace Entropy::Global;
using namespace Entropy::Graphics::Utilities;
using namespace Entropy::Graphics::CommandBuffers;

namespace Entropy
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
                 * @brief Destroy the Buffer object
                 *
                 */
                ~Buffer();

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

                void CreateIndexBufferUint16(std::vector<uint16_t> indices);

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
                static void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory);

                /**
                 * @brief Copy One Buffer into another
                 *
                 * @param srcBuffer VkBuffer source
                 * @param dstBuffer VkBuffer destination
                 * @param size srcBuffer size
                 */
                void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

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
