#include "commandpool.hpp"

using namespace Entropy::Graphics::CommandPools;

CommandPool::CommandPool(std::shared_ptr<LogicalDevice> logicalDevice, std::shared_ptr<PhysicalDevice> physicalDevice, std::shared_ptr<WindowSurface> surface)
{
    QueueFamilyIndices queueFamilyIndices = QueueFamily::FindQueueFamilies(physicalDevice->Get(), surface->Get());

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(logicalDevice->Get(), &poolInfo, nullptr, &_commandPool) != VK_SUCCESS)
    {
        exit(EXIT_FAILURE);
    }
}