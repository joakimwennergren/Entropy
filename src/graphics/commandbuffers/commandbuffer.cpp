#include "commandbuffer.hpp"

using namespace Symbios::Graphics::CommandBuffers;

/**
 * @brief Construct a new Command Buffer:: Command Buffer object
 *
 * @param context
 */
CommandBuffer::CommandBuffer(Context *context)
{
    _context = context;

    Context::QueueFamilyIndices queueFamilyIndices = _context->FindQueueFamilies(_context->GetPhysicalDevice());

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(_context->GetLogicalDevice(), &poolInfo, nullptr, &_commandPool) != VK_SUCCESS)
    {
        PLOG_ERROR << "Failed to create command pool!";
        exit(EXIT_FAILURE);
    }

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = _commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(_context->GetLogicalDevice(), &allocInfo, &_commandBuffer) != VK_SUCCESS)
    {
        PLOG_ERROR << "Failed to allocate command buffer!";
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Destroy the Command Buffer:: Command Buffer object
 *
 */
CommandBuffer::~CommandBuffer()
{
    vkDestroyCommandPool(_context->GetLogicalDevice(), _commandPool, nullptr);
}

/**
 * @brief
 *
 * @param imageIndex
 * @param renderPass
 */
void CommandBuffer::Record(uint32_t imageIndex)
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

/**
 * @brief
 *
 */
void CommandBuffer::EndRecording()
{
    if (vkEndCommandBuffer(_commandBuffer) != VK_SUCCESS)
    {
        PLOG_ERROR << "Failed to end recording command buffer!";
        exit(EXIT_FAILURE);
    }
}
