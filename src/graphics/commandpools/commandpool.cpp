#include "commandpool.hpp"

using namespace Entropy::Graphics::CommandPools;

/**
 * @brief Constructor for CommandPool
 * @param logicalDevice LogicalDevice device
 * @param physicalDevice PhysicalDevice physical device
 * @param surface WindowSurface surface
 */
CommandPool::CommandPool(std::shared_ptr<LogicalDevice> logicalDevice, std::shared_ptr<PhysicalDevice> physicalDevice, std::shared_ptr<WindowSurface> surface)
{
    assert(logicalDevice != nullptr);
    assert(physicalDevice != nullptr);
    assert(surface != nullptr);

    QueueFamilyIndices queueFamilyIndices = QueueFamily::FindQueueFamilies(physicalDevice->Get(), surface->Get());

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.presentFamily.value();

    if (vkCreateCommandPool(logicalDevice->Get(), &poolInfo, nullptr, &_commandPool) != VK_SUCCESS)
    {
        spdlog::error("[CommandPool] Failed to create a command pool.");
    }
}
