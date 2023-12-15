#include "commandbuffer.hpp"

using namespace Entropy::Graphics::CommandBuffers;

CommandBuffer::CommandBuffer(std::shared_ptr<ServiceLocator> serviceLocator)
{
    // Get required depenencies
    auto commandPool = std::dynamic_pointer_cast<CommandPool>(serviceLocator->getService("CommandPool"));
    auto logicalDevice = std::dynamic_pointer_cast<LogicalDevice>(serviceLocator->getService("LogicalDevice"));

    if (!commandPool->isValid() || !logicalDevice->isValid())
    {
        spdlog::error("Trying to create commandbuffer with invalid swapchain or invalid logical device");
        return;
    }

    _logicalDevice = logicalDevice;

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool->Get();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(logicalDevice->Get(), &allocInfo, &_commandBuffer) != VK_SUCCESS)
    {
        exit(EXIT_FAILURE);
    }
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
        exit(EXIT_FAILURE);
    }
}

void CommandBuffer::EndRecording()
{
    if (vkEndCommandBuffer(_commandBuffer) != VK_SUCCESS)
    {
        exit(EXIT_FAILURE);
    }
}

void CommandBuffer::EndRecordingOnce()
{
    if (vkEndCommandBuffer(_commandBuffer) != VK_SUCCESS)
    {
        exit(EXIT_FAILURE);
    }

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &_commandBuffer;

    vkQueueSubmit(_logicalDevice->GetPresentQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(_logicalDevice->GetGraphicQueue());
}
