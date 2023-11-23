#include "buffer.hpp"

using namespace Symbios::Graphics::Buffers;

Buffer::~Buffer()
{
    // Get vulkan context
    VulkanContext *vkContext = VulkanContext::GetInstance();

    // Destroy the buffer
    vkDestroyBuffer(vkContext->logicalDevice, _buffer, nullptr);

    // Free buffer memory
    vkFreeMemory(vkContext->logicalDevice, _bufferMemory, nullptr);
}

void Buffer::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory)
{
    // Get vulkan context
    VulkanContext *vkContext = VulkanContext::GetInstance();

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(vkContext->logicalDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(vkContext->logicalDevice, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = Utility::FindMemoryTypeIndex(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(vkContext->logicalDevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(vkContext->logicalDevice, buffer, bufferMemory, 0);
}

void Buffer::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
    VulkanContext *vkContext = VulkanContext::GetInstance();

    // Create a commandBuffer
    auto cmdBuffer = CommandBuffer();
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
    vkQueueSubmit(vkContext->graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);

    // Wait until GPU processed queue
    vkQueueWaitIdle(vkContext->graphicsQueue);
}

void Buffer::CreateIndexBufferUint16(std::vector<uint16_t> indices)
{

    VulkanContext *vkContext = VulkanContext::GetInstance();

    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void *data;
    vkMapMemory(vkContext->logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t)bufferSize);
    vkUnmapMemory(vkContext->logicalDevice, stagingBufferMemory);

    CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _buffer, _bufferMemory);

    CopyBuffer(stagingBuffer, _buffer, bufferSize);

    vkDestroyBuffer(vkContext->logicalDevice, stagingBuffer, nullptr);
    vkFreeMemory(vkContext->logicalDevice, stagingBufferMemory, nullptr);
}
