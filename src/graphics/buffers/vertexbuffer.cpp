#include "vertexbuffer.hpp"

using namespace Entropy::Graphics::Buffers;

VertexBuffer::VertexBuffer(std::shared_ptr<ServiceLocator> serviceLocator, std::vector<Vertex> vertices)
{
    auto logicalDevice = serviceLocator->GetService<LogicalDevice>();

    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    CreateBuffer(serviceLocator, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void *data;
    vkMapMemory(logicalDevice->Get(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t)bufferSize);
    vkUnmapMemory(logicalDevice->Get(), stagingBufferMemory);

    CreateBuffer(serviceLocator, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _buffer, _bufferMemory);

    CopyBuffer(serviceLocator, stagingBuffer, _buffer, bufferSize);

    vkDestroyBuffer(logicalDevice->Get(), stagingBuffer, nullptr);
    vkFreeMemory(logicalDevice->Get(), stagingBufferMemory, nullptr);
}
