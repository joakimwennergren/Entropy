#include "buffer.hpp"

using namespace Entropy::Graphics::Buffers;

Buffer::~Buffer()
{
    vkDeviceWaitIdle(_logicalDevice->Get());

    // Destroy the buffer
    vkDestroyBuffer(_logicalDevice->Get(), _buffer, nullptr);

    // Free buffer memory
    vkFreeMemory(_logicalDevice->Get(), _bufferMemory, nullptr);
}

void Buffer::CreateBuffer(std::shared_ptr<ServiceLocator> serviceLocator, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory)
{
    // Get required depenencies
    auto logicalDevice = serviceLocator->GetService<LogicalDevice>();

    _logicalDevice = logicalDevice;

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(logicalDevice->Get(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(logicalDevice->Get(), buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = Utility::FindMemoryTypeIndex(serviceLocator, memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(logicalDevice->Get(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(logicalDevice->Get(), buffer, bufferMemory, 0);
}

void Buffer::CopyBuffer(std::shared_ptr<ServiceLocator> serviceLocator, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
    auto logicalDevice = serviceLocator->GetService<LogicalDevice>();

    // Create a commandBuffer
    auto cmdBuffer = CommandBuffer(serviceLocator);
    auto cmdBufferHandle = cmdBuffer.GetCommandBuffer();

    // Start recording
    cmdBuffer.Record();

    // Copy buffer from src to dst
    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(cmdBufferHandle, srcBuffer, dstBuffer, 1, &copyRegion);

    // End recording
    cmdBuffer.EndRecording();

    // Submit to queue
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmdBufferHandle;
    vkQueueSubmit(logicalDevice->GetGraphicQueue(), 1, &submitInfo, VK_NULL_HANDLE);

    // Wait until GPU processed queue
    vkQueueWaitIdle(logicalDevice->GetGraphicQueue());
}

void Buffer::CreateIndexBufferUint16(std::shared_ptr<ServiceLocator> serviceLocator, std::vector<uint16_t> indices)
{
    auto logicalDevice = serviceLocator->GetService<LogicalDevice>();

    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    CreateBuffer(serviceLocator, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void *data;
    vkMapMemory(logicalDevice->Get(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t)bufferSize);
    vkUnmapMemory(logicalDevice->Get(), stagingBufferMemory);

    CreateBuffer(serviceLocator, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _buffer, _bufferMemory);

    CopyBuffer(serviceLocator, stagingBuffer, _buffer, bufferSize);

    vkDestroyBuffer(logicalDevice->Get(), stagingBuffer, nullptr);
    vkFreeMemory(logicalDevice->Get(), stagingBufferMemory, nullptr);
}

void Buffer::CreateIndexBufferUint32(std::shared_ptr<ServiceLocator> serviceLocator, std::vector<uint32_t> indices)
{
    auto logicalDevice = serviceLocator->GetService<LogicalDevice>();

    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    CreateBuffer(serviceLocator, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void *data;
    vkMapMemory(logicalDevice->Get(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t)bufferSize);
    vkUnmapMemory(logicalDevice->Get(), stagingBufferMemory);

    CreateBuffer(serviceLocator, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _buffer, _bufferMemory);

    CopyBuffer(serviceLocator, stagingBuffer, _buffer, bufferSize);

    vkDestroyBuffer(logicalDevice->Get(), stagingBuffer, nullptr);
    vkFreeMemory(logicalDevice->Get(), stagingBufferMemory, nullptr);
}
