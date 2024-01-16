#include "renderer.hpp"

using namespace Entropy::Graphics::Renderers;

Renderer::Renderer(std::shared_ptr<ServiceLocator> serviceLocator)
{
    // Store service locator
    _serviceLocator = serviceLocator;

    // Get required depenencies
    _descriptorSet = std::dynamic_pointer_cast<Descriptorset>(serviceLocator->getService("DescriptorSet"));
    _logicalDevice = std::dynamic_pointer_cast<LogicalDevice>(serviceLocator->getService("LogicalDevice"));
    _swapChain = std::dynamic_pointer_cast<Swapchain>(serviceLocator->getService("SwapChain"));
    _sceneGraph = std::dynamic_pointer_cast<SceneGraph>(serviceLocator->getService("SceneGraph"));

    // Create renderpass
    _renderPass = std::make_shared<RenderPass>(serviceLocator);

    // Create pipeline(s)
    _pipeline = std::make_unique<Pipeline>(_renderPass, serviceLocator);

    // Create synchronizer
    _synchronizer = std::make_unique<Synchronizer>(MAX_CONCURRENT_FRAMES_IN_FLIGHT, serviceLocator);

    for (uint32_t i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++)
    {
        _commandBuffers.push_back(std::make_shared<CommandBuffer>(serviceLocator));
    }

    // Create buffers @todo temp!!!
    for (size_t i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++)
    {
        _uniformBuffers.push_back(new Entopy::Graphics::Buffers::UniformBuffer(serviceLocator, sizeof(UniformBufferObject)));
    }

    for (unsigned int i = 0; i < _uniformBuffers.size(); i++)
    {
        auto rawBuffer = _uniformBuffers[i]->GetVulkanBuffer();
        rawUniformBuffers.push_back(rawBuffer);
    }

    for (size_t i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++)
    {
        std::array<VkWriteDescriptorSet, 1> descriptorWrites{};
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = rawUniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = _descriptorSet->Get()[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;

        vkUpdateDescriptorSets(_logicalDevice->Get(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
}

Renderer::Renderer(uint32_t *vertContent, uint32_t vertSize, uint32_t *fragContent, uint32_t fragSize)
{
    /*
    VulkanContext *vkContext = VulkanContext::GetInstance();

    // Create renderpass
    _renderPass = std::make_shared<RenderPass>();

    // Create pipeline(s)
    _pipeline = std::make_unique<Pipeline>(_renderPass, vertContent, vertSize, fragContent, fragSize);

    _synchronizer = std::make_unique<Synchronizer>(MAX_CONCURRENT_FRAMES_IN_FLIGHT);

    for (uint32_t i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++)
    {
        _commandBuffers.push_back(std::make_shared<CommandBuffer>());
    }

    // Create buffers @todo temp!!!
    for (size_t i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++)
    {
        _uniformBuffers.push_back(new Entopy::Graphics::Buffers::UniformBuffer(sizeof(UniformBufferObject)));
    }

    for (unsigned int i = 0; i < _uniformBuffers.size(); i++)
    {
        auto rawBuffer = _uniformBuffers[i]->GetVulkanBuffer();
        rawUniformBuffers.push_back(rawBuffer);
    }

    for (size_t i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++)
    {
        std::array<VkWriteDescriptorSet, 1> descriptorWrites{};
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = rawUniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = vkContext->descriptorSets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;

        vkUpdateDescriptorSets(vkContext->logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
    */
}

void Renderer::Render(int width, int height)
{
    vkWaitForFences(_logicalDevice->Get(), 1, &_synchronizer->GetFences()[_currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(_logicalDevice->Get(), _swapChain->Get(), UINT64_MAX, _synchronizer->GetImageSemaphores()[_currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
    {
        _synchronizer.reset();
        _synchronizer = std::make_unique<Synchronizer>(MAX_CONCURRENT_FRAMES_IN_FLIGHT, _serviceLocator);
        _swapChain->RecreateSwapChain();
        _renderPass->RecreateFrameBuffers();

        return;
    }
    else if (result != VK_SUCCESS)
    {
        exit(EXIT_FAILURE);
    }

    // current commandbuffer & descriptorset
    currentCmdBuffer = _commandBuffers[_currentFrame]->GetCommandBuffer();
    currentDescriptorSet = _descriptorSet->Get()[_currentFrame];

    // Reset fences and current commandbuffer
    vkResetFences(_logicalDevice->Get(), 1, &_synchronizer->GetFences()[_currentFrame]);
    vkResetCommandBuffer(currentCmdBuffer, 0);

    // Begin renderpass and commandbuffer recording
    _commandBuffers[_currentFrame]->Record();
    _renderPass->Begin(_commandBuffers[_currentFrame], imageIndex);

    // Bind current pipeline
    vkCmdBindPipeline(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline->GetPipeline());

    // Set Viewport
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)_swapChain->swapChainExtent.width;
    viewport.height = (float)_swapChain->swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(currentCmdBuffer, 0, 1, &viewport);

    // Scissor
    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = _swapChain->swapChainExtent;
    vkCmdSetScissor(currentCmdBuffer, 0, 1, &scissor);

    // Sort renderables based on Zindex
    sort(_sceneGraph->renderables.begin(),
         _sceneGraph->renderables.end(),
         [](const std::shared_ptr<Renderable> &lhs, const std::shared_ptr<Renderable> &rhs)
         { return lhs->zIndex < rhs->zIndex; });

    for (auto &renderable : _sceneGraph->renderables)
    {
        if (renderable->children.size() == 0)
        {
            DrawRenderable(renderable, width, height);
        }
        else
        {
            for (auto &child : renderable->children)
            {
                DrawRenderable(child, width, height);
            }
        }
    }

    // End renderpass and commandbuffer recording
    _renderPass->End(_commandBuffers[_currentFrame]);
    _commandBuffers[_currentFrame]->EndRecording();

    // Submit and present
    this->SubmitAndPresent(currentCmdBuffer, imageIndex);

    // Increment current frame
    _currentFrame = (_currentFrame + 1) % MAX_CONCURRENT_FRAMES_IN_FLIGHT;
}

void Renderer::DrawRenderable(std::shared_ptr<Renderable> renderable, int width, int height)
{
    if (!renderable->isAbleToRender())
    {
        return;
    }

    // Update UBO
    UniformBufferObject ubo{};
    ubo.view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    ubo.proj = glm::ortho(0.0f, (float)_swapChain->swapChainExtent.width, (float)_swapChain->swapChainExtent.height, 0.0f, -1.0f, 1.0f);
    ubo.proj[1][1] *= -1;

    ubo.screen = glm::vec2((float)width, (float)height);

    memcpy(_uniformBuffers[_currentFrame]->GetMappedMemory(), &ubo, sizeof(ubo));

    // Bind vertex & index buffers
    // Bind descriptor sets
    VkBuffer vertexBuffers[] = {renderable->vertexBuffer->GetVulkanBuffer()};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(currentCmdBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(currentCmdBuffer, renderable->indexBuffer->GetVulkanBuffer(), 0, VK_INDEX_TYPE_UINT16);
    vkCmdBindDescriptorSets(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline->GetPipelineLayout(), 0, 1, &currentDescriptorSet, 0, nullptr);

    if (renderable->_descriptorSet != nullptr)
        vkCmdBindDescriptorSets(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline->GetPipelineLayout(), 1, 1, &renderable->_descriptorSet, 0, nullptr);

    // @todo refactors this

    auto translate = glm::mat4(1.0f);
    if (renderable->type == 0)
    {
        translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0));
    }
    else
    {
        translate = glm::translate(glm::mat4(1.0f), renderable->position);
    }

    auto scale = glm::scale(glm::mat4(1.0f), renderable->scale);

    auto rotate = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));

    auto o = glm::vec3(0.0, 0.0, 0.0);

    if (renderable->orientation == 1)
    {
        o.x = 1;
    }

    if (renderable->orientation == 2)
    {
        o.y = 1;
    }

    if (renderable->orientation == 3)
    {
        o.z = 1;
    }

    auto modelRotation = glm::rotate(glm::mat4(1.0f), glm::radians(renderable->rotationX), o);
    // @todo end

    // if (renderable->texture->GetImageView() != nullptr)
    // {

    // }

    glm::mat4 modelMatrix = glm::mat4(1.0f);

    modelMatrix = translate * scale * rotate * modelRotation;

    // Update push constant and upload to GPU
    PushConstant constant;
    constant.modelMatrix = modelMatrix;
    constant.color = renderable->color;
    constant.position = renderable->position;
    constant.textureId = renderable->textureId;

    vkCmdPushConstants(currentCmdBuffer, _pipeline->GetPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstant), &constant);

    // Draw current renderable
    vkCmdDrawIndexed(currentCmdBuffer, renderable->GetIndices().size(), 1, 0, 0, 0);
}

void Renderer::SubmitAndPresent(VkCommandBuffer cmdBuffer, uint32_t imageIndex)
{
    // Submit info
    VkSubmitInfo submitInfo{};
    VkSemaphore signalSemaphores[] = {_synchronizer->GetRenderFinishedSemaphores()[_currentFrame]};
    VkSemaphore waitSemaphores[] = {_synchronizer->GetImageSemaphores()[_currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmdBuffer;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    // Submit queue
    if (vkQueueSubmit(_logicalDevice->GetGraphicQueue(), 1, &submitInfo, _synchronizer->GetFences()[_currentFrame]) != VK_SUCCESS)
    {
        exit(EXIT_FAILURE);
    }

    // PresentInfo
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    // SwapChains
    VkSwapchainKHR swapChains[] = {_swapChain->Get()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    // Present
    if (vkQueuePresentKHR(_logicalDevice->GetPresentQueue(), &presentInfo) != VK_SUCCESS)
    {
        exit(EXIT_FAILURE);
    }
}
