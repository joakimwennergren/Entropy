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

    Symbios::Core::Context::QueueFamilyIndices queueFamilyIndices = _context->FindQueueFamilies(_context->GetPhysicalDevice());

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(_context->GetLogicalDevice(), &poolInfo, nullptr, &_commandPool) != VK_SUCCESS)
    {
        PLOG_ERROR << "Failed to create command pool!";
    }

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = _commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(_context->GetLogicalDevice(), &allocInfo, &_commandBuffer) != VK_SUCCESS)
    {
        PLOG_ERROR << "Failed to allocate command buffer!";
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
void CommandBuffer::Record(uint32_t imageIndex, Symbios::Graphics::RenderPasses::Default *renderPass)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;                  // Optional
    beginInfo.pInheritanceInfo = nullptr; // Optional

    if (vkBeginCommandBuffer(_commandBuffer, &beginInfo) != VK_SUCCESS)
    {
        PLOG_ERROR << "Failed to begin recording command buffer!";
        return;
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass->GetRenderPass();
    renderPassInfo.framebuffer = renderPass->swapChainFramebuffers[imageIndex];

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = _context->GetSwapChainExtent();

    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(_commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
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
    }
}