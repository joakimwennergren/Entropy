#include "commandbuffer.hpp"

using namespace Entropy::Graphics::CommandBuffers;

CommandBuffer::CommandBuffer(std::shared_ptr<ServiceLocator> serviceLocator)
{
    // Get required depenencies
    auto commandPool = serviceLocator->GetService<CommandPool>();
    auto logicalDevice = serviceLocator->GetService<LogicalDevice>();

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
    vkDeviceWaitIdle(_logicalDevice->Get());

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(_commandBuffer, &beginInfo);
}

void CommandBuffer::Record()
{
    vkDeviceWaitIdle(_logicalDevice->Get());

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
