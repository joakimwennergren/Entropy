#include "uniformbuffer.hpp"

Entropy::Graphics::Buffers::UniformBuffer::UniformBuffer(std::shared_ptr<ServiceLocator> serviceLocator, VkDeviceSize bufferSize)
{
    auto logicalDevice = serviceLocator->GetService<LogicalDevice>();

    this->CreateBuffer(serviceLocator, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);

    vmaMapMemory(_allocator->Get(), _allocation, &_mappedMemory);
}
