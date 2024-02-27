#include "uniformbuffer.hpp"

Entropy::Graphics::Buffers::UniformBuffer::UniformBuffer(std::shared_ptr<ServiceLocator> serviceLocator, VkDeviceSize bufferSize)
{
    auto logicalDevice = serviceLocator->GetService<LogicalDevice>();

    this->CreateBuffer(serviceLocator, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, _buffer, _bufferMemory);

    vmaMapMemory(_allocator->Get(), _allocation, &_mappedMemory);

}
