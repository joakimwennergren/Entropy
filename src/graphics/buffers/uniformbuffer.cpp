#include "uniformbuffer.hpp"

Entropy::Graphics::Buffers::UniformBuffer::UniformBuffer(std::shared_ptr<ServiceLocator> serviceLocator, VkDeviceSize bufferSize)
{
    // Get required depenencies
    auto logicalDevice = std::dynamic_pointer_cast<LogicalDevice>(serviceLocator->getService("LogicalDevice"));

    if (!logicalDevice->isValid())
    {
        spdlog::error("Trying to create uniform buffer with invalid logical device");
        return;
    }

    this->CreateBuffer(serviceLocator, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, _buffer, _bufferMemory);

    vkMapMemory(logicalDevice->Get(), _bufferMemory, 0, bufferSize, 0, &_mappedMemory);
}
