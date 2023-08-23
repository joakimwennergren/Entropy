#include "buffer.hpp"

using namespace Symbios::Graphics::Buffers;

Buffer::~Buffer()
{
    vkDestroyBuffer(_context->GetLogicalDevice(), _buffer, nullptr);
    vkFreeMemory(_context->GetLogicalDevice(), _bufferMemory, nullptr);
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
