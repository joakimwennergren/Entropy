#pragma once

#include <cassert>

#include <vulkan/vulkan.hpp>
#include <spdlog/spdlog.h>

#include <graphics/memory/allocator.hpp>

using namespace Entropy::Graphics::Memory;

namespace Entropy
{
    namespace Graphics
    {
        namespace Buffers
        {
            class Buffer
            {
            public:
                ~Buffer();

                void CreateIndexBufferUint16(std::shared_ptr<ServiceLocator> serviceLocator, std::vector<uint16_t> indices);

                void CreateIndexBufferUint32(std::shared_ptr<ServiceLocator> serviceLocator, std::vector<uint32_t> indices);

                inline VkBuffer GetVulkanBuffer() { return this->_buffer; };

                inline VkDeviceMemory GetBufferMemory() { return this->_bufferMemory; };

                inline void *GetMappedMemory() { return this->_mappedMemory; };

            protected:
                void CreateBuffer(std::shared_ptr<ServiceLocator> serviceLocator, VkDeviceSize size, VkBufferUsageFlags usage);

                VkBuffer _buffer;
                VkDeviceMemory _bufferMemory;
                VmaAllocation _allocation;
                void *_mappedMemory;
                std::shared_ptr<LogicalDevice> _logicalDevice;
                std::shared_ptr<Allocator> _allocator;
            };
        }
    }
}
