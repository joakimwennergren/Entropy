#include "stagedbuffer.hpp"

using namespace Entropy::Graphics::Buffers;

StagedBuffer::StagedBuffer(std::shared_ptr<ServiceLocator> serviceLocator, VkDeviceSize size, uint8_t *dataIn)
{
    auto logicalDevice = serviceLocator->GetService<LogicalDevice>();

    CreateBuffer(serviceLocator, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, _buffer, _bufferMemory);

    void *data;
    vkMapMemory(logicalDevice->Get(), _bufferMemory, 0, size, 0, &data);
    std::memmove(data, dataIn, static_cast<size_t>(size));
    vkUnmapMemory(logicalDevice->Get(), _bufferMemory);
}