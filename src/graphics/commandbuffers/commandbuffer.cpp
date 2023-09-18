#include "commandbuffer.hpp"

using namespace Symbios::Graphics::CommandBuffers;

CommandBuffer::CommandBuffer()
{
    // Store vulkan ctx
    _context = Global::GetInstance()->GetVulkanContext();

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = _context->GetCommandPool();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(_context->GetLogicalDevice(), &allocInfo, &_commandBuffer) != VK_SUCCESS)
    {
        PLOG_ERROR << "Failed to allocate command buffer!";
        exit(EXIT_FAILURE);
    }
}

CommandBuffer::~CommandBuffer()
{
}

void CommandBuffer::RecordOnce()
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(_commandBuffer, &beginInfo);
}

void CommandBuffer::Record()
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;                  // Optional
    beginInfo.pInheritanceInfo = nullptr; // Optional

    if (vkBeginCommandBuffer(_commandBuffer, &beginInfo) != VK_SUCCESS)
    {
        PLOG_ERROR << "Failed to begin recording command buffer!";
        exit(EXIT_FAILURE);
    }
}

void CommandBuffer::EndRecording()
{
    if (vkEndCommandBuffer(_commandBuffer) != VK_SUCCESS)
    {
        PLOG_ERROR << "Failed to end recording command buffer!";
        exit(EXIT_FAILURE);
    }
}

void CommandBuffer::EndRecordingOnce()
{
    if (vkEndCommandBuffer(_commandBuffer) != VK_SUCCESS)
    {
        PLOG_ERROR << "Failed to end recording command buffer!";
        exit(EXIT_FAILURE);
    }

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &_commandBuffer;

    vkQueueSubmit(_context->GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(_context->GetGraphicsQueue());
}