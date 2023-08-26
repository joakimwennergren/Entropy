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

    _imageAvailableSemaphores.resize(MAX_CONCURRENT_FRAMES_IN_FLIGHT);
    _renderFinishedSemaphores.resize(MAX_CONCURRENT_FRAMES_IN_FLIGHT);
    _inFlightFences.resize(MAX_CONCURRENT_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++)
    {
        if (vkCreateSemaphore(_context->GetLogicalDevice(), &semaphoreInfo, nullptr, &_imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(_context->GetLogicalDevice(), &semaphoreInfo, nullptr, &_renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(_context->GetLogicalDevice(), &fenceInfo, nullptr, &_inFlightFences[i]) != VK_SUCCESS)
        {

            PLOG_ERROR << "failed to create synchronization objects for a frame!";
            exit(EXIT_FAILURE);
        }
    }

    // Create buffers @todo temp!!!
    _vertexBuffer = std::make_unique<Buffer>(_context);
    _vertexBuffer->CreateVertexBuffer(vertices);

    _indexBuffer = std::make_unique<Buffer>(_context);
    _indexBuffer->CreateIndexBufferUint16(indices);

    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    uniformBuffers.resize(MAX_CONCURRENT_FRAMES_IN_FLIGHT);
    uniformBuffersMemory.resize(MAX_CONCURRENT_FRAMES_IN_FLIGHT);
    uniformBuffersMapped.resize(MAX_CONCURRENT_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++)
    {
        Buffer::CreateBuffer(_context, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);

        vkMapMemory(_context->GetLogicalDevice(), uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
    }

    _texture = std::make_unique<Texture>(_context);
    _texture->CreateTextureImage("/Users/joakim/Desktop/Symbios/resources/textures/ivysaur.png");
    _context->CreateDescriptorSets(uniformBuffers, _texture->GetImageView());

    _pipeline->Build();
}

Renderer::~Renderer()
{
    for (size_t i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroyBuffer(_context->GetLogicalDevice(), uniformBuffers[i], nullptr);
        vkFreeMemory(_context->GetLogicalDevice(), uniformBuffersMemory[i], nullptr);
    }

    for (size_t i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroySemaphore(_context->GetLogicalDevice(), _renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(_context->GetLogicalDevice(), _imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(_context->GetLogicalDevice(), _inFlightFences[i], nullptr);
    }
}

void Renderer::Render()
{

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(_context->GetLogicalDevice(), _context->GetSwapChain(), UINT64_MAX, _imageAvailableSemaphores[_currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _framebufferResized)
    {
        _context->RecreateSwapChain();

        _imageAvailableSemaphores.clear();

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++)
        {
            if (vkCreateSemaphore(_context->GetLogicalDevice(), &semaphoreInfo, nullptr, &_imageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(_context->GetLogicalDevice(), &semaphoreInfo, nullptr, &_renderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(_context->GetLogicalDevice(), &fenceInfo, nullptr, &_inFlightFences[i]) != VK_SUCCESS)
            {

                PLOG_ERROR << "failed to create synchronization objects for a frame!";
                exit(EXIT_FAILURE);
            }
        }

        _renderPass->RecreateFrameBuffers();

        _framebufferResized = false;

        return;
    }
    else if (result != VK_SUCCESS)
    {
        PLOG_ERROR << "Failed to acquire swap chain image!";
        exit(EXIT_FAILURE);
    }

    auto currentCmdBuffer = _commandBuffers[_currentFrame]->GetCommandBuffer();

    vkWaitForFences(_context->GetLogicalDevice(), 1, &_inFlightFences[_currentFrame], VK_TRUE, UINT64_MAX);

    // Only reset the fence if we are submitting work
    vkResetFences(_context->GetLogicalDevice(), 1, &_inFlightFences[_currentFrame]);

    vkResetCommandBuffer(currentCmdBuffer, 0);

    _commandBuffers[_currentFrame]->Record();

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

    VkBuffer vertexBuffers[] = {_vertexBuffer->GetBuffer()};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(currentCmdBuffer, 0, 1, vertexBuffers, offsets);

    vkCmdBindIndexBuffer(currentCmdBuffer, _indexBuffer->GetBuffer(), 0, VK_INDEX_TYPE_UINT16);

    auto currentDescriptorSet = _context->GetDescriptorSets()[_currentFrame];

    vkCmdBindDescriptorSets(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline->GetPipelineLayout(), 0, 1, &currentDescriptorSet, 0, nullptr);

    vkCmdDrawIndexed(currentCmdBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

    _renderPass->End(_commandBuffers[_currentFrame]);

    _commandBuffers[_currentFrame]->EndRecording();

    UpdateUniforms(_currentFrame);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {_imageAvailableSemaphores[_currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &currentCmdBuffer;

    VkSemaphore signalSemaphores[] = {_renderFinishedSemaphores[_currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(_context->GetGraphicsQueue(), 1, &submitInfo, _inFlightFences[_currentFrame]) != VK_SUCCESS)
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

    vkQueuePresentKHR(_context->GetPresentQueue(), &presentInfo);

    _currentFrame = (_currentFrame + 1) % MAX_CONCURRENT_FRAMES_IN_FLIGHT;
}

void Renderer::UpdateUniforms(uint32_t currentImage)
{
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 1.0f));

    // ubo.model = glm::mat4(1.0f);
    ubo.view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // ubo.proj = glm::ortho(0.0F, (float)_context->GetSwapChainExtent().width, (float)_context->GetSwapChainExtent().height, 0.0F);
    //  ubo.proj[1][1] *= -1;
    //   ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    ubo.proj = glm::perspective(glm::radians(45.0f), _context->GetSwapChainExtent().width / (float)_context->GetSwapChainExtent().height, 0.1f, 10.0f);

    ubo.proj[1][1] *= -1;

    memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}
