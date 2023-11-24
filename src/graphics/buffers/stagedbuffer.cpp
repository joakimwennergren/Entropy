#include "stagedbuffer.hpp"

using namespace Entropy::Graphics::Buffers;

StagedBuffer::StagedBuffer(VkDeviceSize size, uint8_t *dataIn)
{
    VulkanContext *vkContext = VulkanContext::GetInstance();

    CreateBuffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, _buffer, _bufferMemory);

    void *data;
    vkMapMemory(vkContext->logicalDevice, _bufferMemory, 0, size, 0, &data);
    memcpy(data, dataIn, static_cast<size_t>(size));
    vkUnmapMemory(vkContext->logicalDevice, _bufferMemory);
}