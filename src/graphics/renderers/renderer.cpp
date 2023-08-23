#include "renderer.hpp"

using namespace Symbios::Graphics::Renderers;

Renderer::Renderer(std::shared_ptr<Context> context)
{
    _context = context;

    _renderPass = std::make_shared<RenderPass>(_context);

    _pipeline = std::make_unique<Pipeline>(_context, _renderPass);

    for (uint32_t i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++)
    {
        _commandBuffers.push_back(std::make_shared<CommandBuffer>(_context));
    }

    imageAvailableSemaphores.resize(MAX_CONCURRENT_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_CONCURRENT_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_CONCURRENT_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++)
    {
        if (vkCreateSemaphore(_context->GetLogicalDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(_context->GetLogicalDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(_context->GetLogicalDevice(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS)
        {

            PLOG_ERROR << "failed to create synchronization objects for a frame!";
            exit(EXIT_FAILURE);
        }
    }
}

Renderer::~Renderer()
{
    for (size_t i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroySemaphore(_context->GetLogicalDevice(), renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(_context->GetLogicalDevice(), imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(_context->GetLogicalDevice(), inFlightFences[i], nullptr);
    }
}

void Renderer::Render()
{

    auto currentCmdBuffer = _commandBuffers[_currentFrame]->GetCommandBuffer();

    vkWaitForFences(_context->GetLogicalDevice(), 1, &inFlightFences[_currentFrame], VK_TRUE, UINT64_MAX);
    vkResetFences(_context->GetLogicalDevice(), 1, &inFlightFences[_currentFrame]);

    uint32_t imageIndex;
    vkAcquireNextImageKHR(_context->GetLogicalDevice(), _context->GetSwapChain(), UINT64_MAX, imageAvailableSemaphores[_currentFrame], VK_NULL_HANDLE, &imageIndex);

    vkResetCommandBuffer(currentCmdBuffer, /*VkCommandBufferResetFlagBits*/ 0);

    _commandBuffers[_currentFrame]->Record(imageIndex);

    _renderPass->Begin(_commandBuffers[_currentFrame], imageIndex);

    vkCmdBindPipeline(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline->GetPipeline());

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)_context->GetSwapChainExtent().width;
    viewport.height = (float)_context->GetSwapChainExtent().height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(currentCmdBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = _context->GetSwapChainExtent();
    vkCmdSetScissor(currentCmdBuffer, 0, 1, &scissor);

    vkCmdDraw(currentCmdBuffer, 3, 1, 0, 0);

    _renderPass->End(_commandBuffers[_currentFrame]);

    _commandBuffers[_currentFrame]->EndRecording();

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[_currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &currentCmdBuffer;

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[_currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(_context->_graphicsQueue, 1, &submitInfo, inFlightFences[_currentFrame]) != VK_SUCCESS)
    {
        PLOG_ERROR << "failed to submit draw command buffer!";
        exit(EXIT_FAILURE);
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

    _currentFrame = (_currentFrame + 1) % MAX_CONCURRENT_FRAMES_IN_FLIGHT;
}
