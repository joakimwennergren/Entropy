#include "vertexbuffer.hpp"

using namespace Entropy::Graphics::Buffers;

VertexBuffer::VertexBuffer(std::shared_ptr<ServiceLocator> serviceLocator, std::vector<Vertex> vertices)
{
    auto logicalDevice = serviceLocator->GetService<LogicalDevice>();

    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    CreateBuffer(serviceLocator, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

    void *mappedData;
    vmaMapMemory(_allocator->Get(), _allocation, &mappedData);
    memcpy(mappedData, vertices.data(), bufferSize);
    vmaUnmapMemory(_allocator->Get(), _allocation);
}
