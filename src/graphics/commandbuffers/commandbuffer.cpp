#include "commandbuffer.hpp"

using namespace Entropy::Graphics::CommandBuffers;

CommandBuffer::CommandBuffer(std::shared_ptr<ServiceLocator> serviceLocator, VkCommandBufferLevel level)
{
    // Get required depenencies
    auto logicalDevice = serviceLocator->GetService<LogicalDevice>();
    auto physicalDevice = serviceLocator->GetService<PhysicalDevice>();
    auto surface = serviceLocator->GetService<WindowSurface>();

    _logicalDevice = logicalDevice;
    _queueSync = serviceLocator->GetService<QueueSync>();

    _pool = std::make_unique<CommandPool>(logicalDevice, physicalDevice, surface);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = _pool->Get();
    allocInfo.level = level;
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(logicalDevice->Get(), &allocInfo, &_commandBuffer) != VK_SUCCESS)
    {
        exit(EXIT_FAILURE);
    }
}

void CommandBuffer::RecordOnce()
{
    // vkDeviceWaitIdle(_logicalDevice->Get());

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(_commandBuffer, &beginInfo);
}

void CommandBuffer::Record()
{
    // vkDeviceWaitIdle(_logicalDevice->Get());

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
    assert(_commandBuffer != VK_NULL_HANDLE);

    if (vkEndCommandBuffer(_commandBuffer) != VK_SUCCESS)
    {
        spdlog::warn("[CommandBuffer] Error while end recording.");
    }
}

void CommandBuffer::EndRecordingOnce()
{
    assert(_commandBuffer != VK_NULL_HANDLE);

    if (vkEndCommandBuffer(_commandBuffer) != VK_SUCCESS)
    {
        spdlog::warn("[CommandBuffer] Error while end recording once.");
    }
}
