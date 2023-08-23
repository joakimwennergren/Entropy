#include "buffer.hpp"

using namespace Symbios::Graphics::Buffers;

Buffer::Buffer(std::shared_ptr<Context> context, uint32_t size)
{
    _context = context;

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
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
}

Buffer::~Buffer()
{
    vkDestroyBuffer(_context->GetLogicalDevice(), _buffer, nullptr);
}

uint32_t Buffer::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(_context->GetPhysicalDevice(), &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    PLOG_ERROR << "failed to find suitable memory type!";
    exit(EXIT_FAILURE);
}