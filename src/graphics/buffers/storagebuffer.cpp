#include "storagebuffer.hpp"

Entropy::Graphics::Buffers::StorageBuffer::StorageBuffer(std::shared_ptr<ServiceLocator> serviceLocator, VkDeviceSize bufferSize, void *input)
{
    auto logicalDevice = serviceLocator->GetService<LogicalDevice>();

    this->CreateBuffer(serviceLocator, bufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, _buffer, _bufferMemory);

    vkMapMemory(logicalDevice->Get(), _bufferMemory, 0, bufferSize, 0, &_mappedMemory);
}
