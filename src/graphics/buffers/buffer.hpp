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
                Buffer() = default;
                ~Buffer();

                template <typename T>
                inline void New(std::shared_ptr<Context> context, T data)
                {

                    _context = context;

                    VkBufferCreateInfo bufferInfo{};
                    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
                    bufferInfo.size = sizeof(data[0]) * data.size();
                    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
                    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

                    if (vkCreateBuffer(_context->GetLogicalDevice(), &bufferInfo, nullptr, &_buffer) != VK_SUCCESS)
                    {
                        PLOG_ERROR << "Failed to create buffer!";
                        exit(EXIT_FAILURE);
                    }

                    VkMemoryRequirements memRequirements;
                    vkGetBufferMemoryRequirements(_context->GetLogicalDevice(), _buffer, &memRequirements);

                    VkMemoryAllocateInfo allocInfo{};
                    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
                    allocInfo.allocationSize = memRequirements.size;
                    allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

                    if (vkAllocateMemory(_context->GetLogicalDevice(), &allocInfo, nullptr, &_bufferMemory) != VK_SUCCESS)
                    {
                        PLOG_ERROR << "Failed to allocate buffer memory!";
                        exit(EXIT_FAILURE);
                    }

                    vkBindBufferMemory(_context->GetLogicalDevice(), _buffer, _bufferMemory, 0);

                    void *datatemp;
                    vkMapMemory(_context->GetLogicalDevice(), _bufferMemory, 0, bufferInfo.size, 0, &datatemp);
                    memcpy(datatemp, data.data(), (size_t)bufferInfo.size);
                    vkUnmapMemory(_context->GetLogicalDevice(), _bufferMemory);
                }

                inline VkBuffer GetBuffer() { return this->_buffer; };

            private:
                uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
                std::shared_ptr<Context> _context;
                VkBuffer _buffer;
                VkDeviceMemory _bufferMemory;
            };
        }
    }
}
