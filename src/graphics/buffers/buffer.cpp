#include "buffer.hpp"

using namespace Symbios::Graphics::Buffers;

Buffer::Buffer()
{
    // Store vulkan ctx
    _context = Global::VulkanContext::GetInstance()->GetVulkanContext();
}

Buffer::~Buffer()
{
    // Destroy the buffer
    vkDestroyBuffer(_context->GetLogicalDevice(), _buffer, nullptr);

    // Free buffer memory
    vkFreeMemory(_context->GetLogicalDevice(), _bufferMemory, nullptr);
}

void Buffer::CreateBuffer(std::shared_ptr<Context> _context, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(_context->GetLogicalDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(_context->GetLogicalDevice(), buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = Utility::FindMemoryTypeIndex(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(_context->GetLogicalDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(_context->GetLogicalDevice(), buffer, bufferMemory, 0);
}

void Buffer::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
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
    vkQueueSubmit(_context->GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);

    // Wait until GPU processed queue
    vkQueueWaitIdle(_context->GetGraphicsQueue());
}

/**
 * @brief
 *
 * @param indices
 */
void Buffer::CreateIndexBufferUint16(std::vector<uint16_t> indices)
{
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    CreateBuffer(_context, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void *data;
    vkMapMemory(_context->GetLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t)bufferSize);
    vkUnmapMemory(_context->GetLogicalDevice(), stagingBufferMemory);

    CreateBuffer(_context, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _buffer, _bufferMemory);

    CopyBuffer(stagingBuffer, _buffer, bufferSize);

    vkDestroyBuffer(_context->GetLogicalDevice(), stagingBuffer, nullptr);
    vkFreeMemory(_context->GetLogicalDevice(), stagingBufferMemory, nullptr);
}
