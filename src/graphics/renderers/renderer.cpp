#include "renderer.hpp"

using namespace Symbios::Graphics::Renderers;

Renderer::Renderer()
{
    // Store vulkan ctx
    _context = Global::GetInstance()->GetVulkanContext();

    // Create renderpass
    _renderPass = std::make_shared<RenderPass>();

    // Create pipeline(s)
    _pipeline = std::make_unique<Pipeline>(_renderPass);

    // @todo this isnt necessary..
    _pipeline->Build();

    for (uint32_t i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++)
    {
        _commandBuffers.push_back(std::make_shared<CommandBuffer>());
    }

    _imageAvailableSemaphores.resize(MAX_CONCURRENT_FRAMES_IN_FLIGHT);
    _renderFinishedSemaphores.resize(MAX_CONCURRENT_FRAMES_IN_FLIGHT);
    _inFlightFences.resize(MAX_CONCURRENT_FRAMES_IN_FLIGHT);

    // @todo should this be in context or something??
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
    for (size_t i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++)
    {
        _uniformBuffers.push_back(new UniformBuffer(sizeof(UniformBufferObject)));
    }

    ivy7 = new Quad();
    ivy7->position = glm::vec3(500.0, -500.0, 0.0);
    ivy7->scale = glm::vec3(100.0, 100.0, 0.0);
    ivy7->textureId = 2;
    ivy7->texture->CreateTextureImage("/Users/joakim/Desktop/Symbios/resources/textures/banderoll.png");

    //_sprites.push_back(ivy7);

    // @temp dynamic UBO
    // Calculate required alignment based on minimum device offset alignment

    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(_context->GetPhysicalDevice(), &properties);
    VkDeviceSize minUboAlignment = properties.limits.minUniformBufferOffsetAlignment;

    dynamicAlignment = sizeof(glm::mat4);

    if (minUboAlignment > 0)
    {
        dynamicAlignment = (dynamicAlignment + minUboAlignment - 1) & ~(minUboAlignment - 1);
    }

    for (unsigned int i = 0; i < _uniformBuffers.size(); i++)
    {
        auto rawBuffer = _uniformBuffers[i]->GetVulkanBuffer();
        rawUniformBuffers.push_back(rawBuffer);
    }

    _context->CreateDescriptorSets(rawUniformBuffers, ivy7->texture->GetImageView());
}

Renderer::~Renderer()
{
    vkDeviceWaitIdle(_context->GetLogicalDevice());

    for (size_t i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroySemaphore(_context->GetLogicalDevice(), _renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(_context->GetLogicalDevice(), _imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(_context->GetLogicalDevice(), _inFlightFences[i], nullptr);
    }
}

void Renderer::Render(std::shared_ptr<SceneGraph> graph)
{

    float scale = 0.1;

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(_context->GetLogicalDevice(), _context->GetSwapChain(), UINT64_MAX, _imageAvailableSemaphores[_currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _framebufferResized)
    {
        _context->RecreateSwapChain();

        for (size_t i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++)
        {
            vkDestroySemaphore(_context->GetLogicalDevice(), _imageAvailableSemaphores[i], nullptr);
        }

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++)
        {
            if (vkCreateSemaphore(_context->GetLogicalDevice(), &semaphoreInfo, nullptr, &_imageAvailableSemaphores[i]) != VK_SUCCESS)
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

    uint32_t modelCnt = 0;

    sort(graph->renderables.begin(), graph->renderables.end(), [](std::shared_ptr<Renderable> lhs, std::shared_ptr<Renderable> rhs) {
      return lhs->zIndex < rhs->zIndex;
    });

    for (auto sprite : graph->renderables)
    {
        if(!sprite->visible)
            return;
        
        auto currentDescriptorSet = _context->GetDescriptorSets()[_currentFrame];
        vkCmdBindDescriptorSets(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline->GetPipelineLayout(), 0, 1, &currentDescriptorSet, 0, nullptr);

        if (sprite->texture->hasTexture)
            vkCmdBindDescriptorSets(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline->GetPipelineLayout(), 1, 1, &sprite->_descriptorSet, 0, nullptr);

        VkBuffer vertexBuffers[] = {sprite->vertexBuffer->GetVulkanBuffer()};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(currentCmdBuffer, 0, 1, vertexBuffers, offsets);

        vkCmdBindIndexBuffer(currentCmdBuffer, sprite->indexBuffer->GetVulkanBuffer(), 0, VK_INDEX_TYPE_UINT16);

        UniformBufferObject ubo{};

        auto model = glm::mat4(1.0f);

        auto translate = glm::translate(glm::mat4(1.0f), sprite->position);
        auto scale = glm::scale(glm::mat4(1.0), sprite->scale);
        auto rotate = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
        auto modelRotation = glm::rotate(glm::mat4(1.0f), glm::radians(sprite->rotationX), glm::vec3(0.0, 1.0, 0.0));

        InstancePushConstants constants;
        constants.modelMatrix = translate * scale * rotate * modelRotation;
        constants.color = sprite->color;

        if (sprite->texture->GetImageView() != nullptr)
        {
            constants.textureId = sprite->textureId;
        }

        // upload the matrix to the GPU via push constants
        vkCmdPushConstants(currentCmdBuffer, _pipeline->GetPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(InstancePushConstants), &constants);

        ubo.view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        ubo.proj = glm::ortho(0.0f, (float)_context->GetSwapChainExtent().width, (float)_context->GetSwapChainExtent().height, 0.0f, -1.0f, 1.0f);
        ubo.proj[1][1] *= -1;

        memcpy(_uniformBuffers[_currentFrame]->GetMappedMemory(), &ubo, sizeof(ubo));

        vkCmdDrawIndexed(currentCmdBuffer, sprite->GetIndices().size(), 1, 0, 0, 0);

        modelCnt++;
    }

    _renderPass->End(_commandBuffers[_currentFrame]);

    _commandBuffers[_currentFrame]->EndRecording();

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
