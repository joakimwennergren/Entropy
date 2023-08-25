#include "commandbuffer.hpp"

using namespace Symbios::Graphics::CommandBuffers;

/**
 * @brief Construct a new Command Buffer:: Command Buffer object
 *
 * @param context
 */
CommandBuffer::CommandBuffer(std::shared_ptr<Context> context)
{
    _context = context;

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

/**
 * @brief Destroy the Command Buffer:: Command Buffer object
 *
 */
CommandBuffer::~CommandBuffer()
{
}

/**
 * @brief
 *
 */
void CommandBuffer::RecordOnce()
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(_commandBuffer, &beginInfo);
}

/**
 * @brief
 *
 * @param imageIndex
 * @param renderPass
 */
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
