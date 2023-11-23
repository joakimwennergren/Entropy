#include "stagedbuffer.hpp"

using namespace Symbios::Graphics::Buffers;

StagedBuffer::StagedBuffer(VkDeviceSize size, uint8_t *dataIn)
{
    // Store vulkan ctx
    _context = VulkanContext::GetInstance()->GetVulkanContext();

    CreateBuffer(_context, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, _buffer, _bufferMemory);

    void *data;
    vkMapMemory(_context->GetLogicalDevice(), _bufferMemory, 0, size, 0, &data);
    memcpy(data, dataIn, static_cast<size_t>(size));
    vkUnmapMemory(_context->GetLogicalDevice(), _bufferMemory);
}

StagedBuffer::~StagedBuffer()
{
}
