#include "vertexbuffer.hpp"

using namespace Symbios::Graphics::Buffers;

VertexBuffer::VertexBuffer(std::vector<Vertex> vertices)
{
    VulkanContext *vkContext = VulkanContext::GetInstance();

    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void *data;
    vkMapMemory(vkContext->logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t)bufferSize);
    vkUnmapMemory(vkContext->logicalDevice, stagingBufferMemory);

    CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _buffer, _bufferMemory);

    CopyBuffer(stagingBuffer, _buffer, bufferSize);

    vkDestroyBuffer(vkContext->logicalDevice, stagingBuffer, nullptr);
    vkFreeMemory(vkContext->logicalDevice, stagingBufferMemory, nullptr);
}
