#include "renderer.hpp"

using namespace Symbios::Graphics::Renderers;

Renderer::Renderer(Context *context)
{
    _context = context;

    _renderPass = new Symbios::Graphics::RenderPasses::RenderPass(_context);

    _pipeline = new Symbios::Graphics::Pipelines::Pipeline(_context, _renderPass);

    _commandBuffer = new Symbios::Graphics::CommandBuffers::CommandBuffer(_context);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    if (vkCreateSemaphore(_context->GetLogicalDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
        vkCreateSemaphore(_context->GetLogicalDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS ||
        vkCreateFence(_context->GetLogicalDevice(), &fenceInfo, nullptr, &inFlightFence) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create semaphores!");
    }
}

Renderer::~Renderer()
{
}

void Renderer::Render()
{
    vkWaitForFences(_context->GetLogicalDevice(), 1, &inFlightFence, VK_TRUE, UINT64_MAX);
    vkResetFences(_context->GetLogicalDevice(), 1, &inFlightFence);

    uint32_t imageIndex;
    vkAcquireNextImageKHR(_context->GetLogicalDevice(), _context->GetSwapChain(), UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

    vkResetCommandBuffer(_commandBuffer->GetCommandBuffer(), /*VkCommandBufferResetFlagBits*/ 0);

    _commandBuffer->Record(imageIndex);

    _renderPass->Begin(_commandBuffer, imageIndex);

    vkCmdBindPipeline(_commandBuffer->GetCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline->GetPipeline());

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)_context->GetSwapChainExtent().width;
    viewport.height = (float)_context->GetSwapChainExtent().height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(_commandBuffer->GetCommandBuffer(), 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = _context->GetSwapChainExtent();
    vkCmdSetScissor(_commandBuffer->GetCommandBuffer(), 0, 1, &scissor);

    vkCmdDraw(_commandBuffer->GetCommandBuffer(), 3, 1, 0, 0);

    _renderPass->End(_commandBuffer);

    _commandBuffer->EndRecording();

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    auto cmd = _commandBuffer->GetCommandBuffer();

    VkSemaphore waitSemaphores[] = {imageAvailableSemaphore};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmd;

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(_context->_graphicsQueue, 1, &submitInfo, inFlightFence) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {_context->GetSwapChain()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    vkQueuePresentKHR(_context->_presentQueue, &presentInfo);
}
