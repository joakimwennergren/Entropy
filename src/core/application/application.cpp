#include "application.hpp"

/**
 * @brief Construct a new Application:: Application object
 *
 */
Application::Application()
{
    // Initialize logger
    static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::verbose, &consoleAppender);

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_WINDOWS)
    // Initialize GLFW
    if (!glfwInit())
    {
        PLOG_FATAL << "Could not initialize GLFW library!";
        return;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    // Create a windowed mode window
    _window = glfwCreateWindow(640, 480, "Symbios dev application", NULL, NULL);

    if (!_window)
    {
        PLOG_FATAL << "Could not create window!";
        glfwTerminate();
        return;
    }

    // Create Vulkan context
    _context = new Symbios::Core::Context(_window);

    _renderPass = new Symbios::Graphics::RenderPasses::Default(_context);

    _pipeline = new Symbios::Graphics::Pipeline::Default(_context, _renderPass);

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

#endif
}

/**
 * @brief Destroy the Application:: Application object
 *
 */
Application::~Application()
{
#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_WINDOWS)
    glfwDestroyWindow(_window);
    glfwTerminate();
#endif
}

/**
 * @brief
 *
 * @return int
 */
void Application::Run()
{
#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_WINDOWS)

    while (!glfwWindowShouldClose(_window))
    {

        vkWaitForFences(_context->GetLogicalDevice(), 1, &inFlightFence, VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        vkAcquireNextImageKHR(_context->GetLogicalDevice(), _context->GetSwapChain(), UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

        vkResetCommandBuffer(_commandBuffer->GetCommandBuffer(), 0);

        _commandBuffer->Record(0, _renderPass);

        vkCmdBindPipeline(_commandBuffer->GetCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline->pipeline);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(_context->GetSwapChainExtent().width);
        viewport.height = static_cast<float>(_context->GetSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(_commandBuffer->GetCommandBuffer(), 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = _context->GetSwapChainExtent();
        vkCmdSetScissor(_commandBuffer->GetCommandBuffer(), 0, 1, &scissor);

        vkCmdDraw(_commandBuffer->GetCommandBuffer(), 3, 1, 0, 0);

        vkCmdEndRenderPass(_commandBuffer->GetCommandBuffer());

        _commandBuffer->EndRecording();

        auto cmd = _commandBuffer->GetCommandBuffer();

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

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

        presentInfo.pResults = nullptr; // Optional

        vkQueuePresentKHR(_context->_presentQueue, &presentInfo);

        //vkResetFences(_context->GetLogicalDevice(), 1, &inFlightFence);
        
        glfwPollEvents();
    }
#endif
}
