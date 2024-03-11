#include "storagebuffer.hpp"

Entropy::Graphics::Buffers::StorageBuffer::StorageBuffer(std::shared_ptr<ServiceLocator> serviceLocator, VkDeviceSize bufferSize, void *input)
{
    auto logicalDevice = serviceLocator->GetService<LogicalDevice>();

    this->CreateBuffer(serviceLocator, bufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);

    vkMapMemory(logicalDevice->Get(), _bufferMemory, 0, bufferSize, 0, &_mappedMemory);
}
