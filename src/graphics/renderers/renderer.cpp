#include "renderer.hpp"

using namespace Entropy::Graphics::Renderers;

size_t Renderer::pad_uniform_buffer_size(size_t originalSize)
{
    // Calculate required alignment based on minimum device offset alignment
    size_t minUboAlignment = properties.limits.minUniformBufferOffsetAlignment;
    size_t alignedSize = originalSize;
    if (minUboAlignment > 0)
    {
        alignedSize = (alignedSize + minUboAlignment - 1) & ~(minUboAlignment - 1);
    }
    return alignedSize;
}

void Renderer::Setup(std::shared_ptr<ServiceLocator> serviceLocator)
{
    // Store service locator
    _serviceLocator = serviceLocator;

    // Get required depenencies
    _logicalDevice = serviceLocator->GetService<LogicalDevice>();
    _swapChain = serviceLocator->GetService<Swapchain>();
    _sceneGraph = serviceLocator->GetService<SceneGraph>();
    _keyboard = serviceLocator->GetService<Keyboard>();
    _cam = serviceLocator->GetService<Cam>();
    auto physicalDevice = serviceLocator->GetService<PhysicalDevice>();

    // Create synchronizer
    _synchronizer = std::make_unique<Synchronizer>(MAX_CONCURRENT_FRAMES_IN_FLIGHT, serviceLocator);

    for (uint32_t i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++)
    {
        _commandBuffers.push_back(std::make_shared<CommandBuffer>(serviceLocator, VK_COMMAND_BUFFER_LEVEL_PRIMARY));
    }

    // Create buffers @todo temp!!!
    for (size_t i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++)
    {
        _uniformBuffers.push_back(new UniformBuffer(serviceLocator, sizeof(UniformBufferObject)));
        dynUbos.push_back(new UniformBuffer(serviceLocator, sizeof(UboDataDynamic) * 10));
    }

    for (unsigned int i = 0; i < _uniformBuffers.size(); i++)
    {
        auto rawBuffer = _uniformBuffers[i]->GetVulkanBuffer();
        rawUniformBuffers.push_back(rawBuffer);
    }

    vkGetPhysicalDeviceProperties(physicalDevice->Get(), &properties);
    size_t minUboAlignment = properties.limits.minUniformBufferOffsetAlignment;
    dynamicAlignment = sizeof(UboDataDynamic);

    if (minUboAlignment > 0)
    {
        dynamicAlignment = (dynamicAlignment + minUboAlignment - 1) & ~(minUboAlignment - 1);
    }

    for (size_t i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++)
    {
        std::array<VkWriteDescriptorSet, 4> descriptorWrites{};

        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = rawUniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        VkDescriptorBufferInfo bufferInfo2{};
        bufferInfo2.buffer = dynUbos[i]->GetVulkanBuffer();
        bufferInfo2.offset = 0;
        bufferInfo2.range = sizeof(UboDataDynamic);

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = _pipelines["SkinnedPipeline"]->descriptorSets[0]->Get()[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;

        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[1].dstSet = _pipelines["Pipeline2D"]->descriptorSets[0]->Get()[i];
        descriptorWrites[1].dstBinding = 0;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pBufferInfo = &bufferInfo;

        descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[2].dstSet = _pipelines["SkinnedPipeline"]->descriptorSets[0]->Get()[i];
        descriptorWrites[2].dstBinding = 1;
        descriptorWrites[2].dstArrayElement = 0;
        descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        descriptorWrites[2].descriptorCount = 1;
        descriptorWrites[2].pBufferInfo = &bufferInfo2;

        descriptorWrites[3].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[3].dstSet = _pipelines["Pipeline2D"]->descriptorSets[0]->Get()[i];
        descriptorWrites[3].dstBinding = 1;
        descriptorWrites[3].dstArrayElement = 0;
        descriptorWrites[3].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        descriptorWrites[3].descriptorCount = 1;
        descriptorWrites[3].pBufferInfo = &bufferInfo2;

        vkUpdateDescriptorSets(_logicalDevice->Get(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }

    _camera = std::make_shared<Camera>();
    _camera->type = Camera::CameraType::firstperson;
    _camera->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    _camera->setRotation(glm::vec3(0.0f));
}

#ifdef BUILD_FOR_ANDROID

std::vector<char> Renderer::loadShader(std::string filename, AAssetManager *assetManager)
{
    AAsset *file = AAssetManager_open(assetManager,
                                      filename.c_str(), AASSET_MODE_BUFFER);
    size_t fileLength = AAsset_getLength(file);
    char *fileContent = new char[fileLength];
    AAsset_read(file, fileContent, fileLength);
    AAsset_close(file);

    std::vector<char> code;

    for (uint32_t i = 0; i < fileLength; i++)
    {
        code.push_back(fileContent[i]);
    }
    return code;
}

Renderer::Renderer(std::shared_ptr<ServiceLocator> serviceLocator, AAssetManager *assetManager)
{

    auto skinnedVert = loadShader("skinned_vert.spv", assetManager);
    auto skinnedFrag = loadShader("skinned_frag.spv", assetManager);

    auto cubemapVert = loadShader("cubemap_vert.spv", assetManager);
    auto cubemapFrag = loadShader("cubemap_frag.spv", assetManager);

    auto twodVert = loadShader("2d_vert.spv", assetManager);
    auto twodFrag = loadShader("2d_frag.spv", assetManager);

    // Create renderpass
    _renderPass = std::make_shared<RenderPass>(serviceLocator);
    // Create skinned pipeline
    _pipelines["SkinnedPipeline"] = std::make_shared<SkinnedPipeline>(_renderPass, serviceLocator, skinnedVert, skinnedFrag);
    _pipelines["CubeMapPipeline"] = std::make_shared<CubeMapPipeline>(_renderPass, serviceLocator, cubemapVert, cubemapFrag);
    _pipelines["Pipeline2D"] = std::make_shared<Pipeline2D>(_renderPass, serviceLocator, twodVert, twodFrag);
    Setup(serviceLocator);
}
#endif
Renderer::Renderer(std::shared_ptr<ServiceLocator> serviceLocator)
{
    // Create renderpass
    _renderPass = std::make_shared<RenderPass>(serviceLocator);
    // Create skinned pipeline
    _pipelines["SkinnedPipeline"] = std::make_shared<SkinnedPipeline>(_renderPass, serviceLocator);
    _pipelines["CubeMapPipeline"] = std::make_shared<CubeMapPipeline>(_renderPass, serviceLocator);
    _pipelines["Pipeline2D"] = std::make_shared<Pipeline2D>(_renderPass, serviceLocator);
    Setup(serviceLocator);
}

void Renderer::HandleResize()
{
    ZoneScopedN("Resizing");

    if (imageResult == VK_SUBOPTIMAL_KHR || imageResult == VK_ERROR_OUT_OF_DATE_KHR)
    {
        skip = true;
        _synchronizer.reset();
        _synchronizer = std::make_unique<Synchronizer>(MAX_CONCURRENT_FRAMES_IN_FLIGHT, _serviceLocator);
        _swapChain->RecreateSwapChain();
        _renderPass->RecreateDepthBuffer();
        _renderPass->RecreateFrameBuffers();
    }
}

void Renderer::Render(int width, int height, bool resize)
{

    _camera->setPerspective(60.0f, (float)width / (float)height, 0.1f, 256.0f);

    vkWaitForFences(_logicalDevice->Get(), 1, &_synchronizer->GetFences()[_currentFrame], VK_TRUE, UINT64_MAX);

    imageResult = vkAcquireNextImageKHR(_logicalDevice->Get(), _swapChain->Get(), UINT64_MAX, _synchronizer->GetImageSemaphores()[_currentFrame], VK_NULL_HANDLE, &imageIndex);

    // Reset fences and current commandbuffer
    vkResetFences(_logicalDevice->Get(), 1, &_synchronizer->GetFences()[_currentFrame]);

    // current commandbuffer & descriptorset
    currentCmdBuffer = _commandBuffers[_currentFrame]->GetCommandBuffer();

    // vkResetCommandBuffer(currentCmdBuffer, 0);

    if (cmdBufferUI != nullptr)
    {
        auto secondaries = cmdBufferUI->GetCommandBuffer();
        vkCmdExecuteCommands(
            currentCmdBuffer,
            1,
            &secondaries);
    }

    // Begin renderpass and commandbuffer recording
    _commandBuffers[_currentFrame]->Record();
    _renderPass->Begin(_commandBuffers[_currentFrame], imageIndex);

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
    scissor.extent = {_swapChain->swapChainExtent};
    vkCmdSetScissor(currentCmdBuffer, 0, 1, &scissor);

    /*
    // Sort renderables based on Zindex
    sort(_sceneGraph->renderables.begin(),
         _sceneGraph->renderables.end(),
         [](const std::shared_ptr<Renderable> &lhs, const std::shared_ptr<Renderable> &rhs)
         { return lhs->zIndex < rhs->zIndex; });
         */

    UniformBufferObject ubo{};

    ubo.screen = glm::vec2(width, height);

    ubo.view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    ubo.proj = glm::ortho(0.0f, (float)_swapChain->swapChainExtent.width, (float)_swapChain->swapChainExtent.height, 0.0f, -1.0f, 1.0f);
    // ubo.proj[1][1] *= -1;

    uint32_t modelIndex = 0;

    for (auto &renderable : _sceneGraph->renderables)
    {

        renderable->Test();
        // Update UBO

        // ubo.screen = glm::vec2((float)width, (float)height);

        memcpy(_uniformBuffers[_currentFrame]->GetMappedMemory(), &ubo, sizeof(ubo));

        if (renderable->children.size() == 0)
        {
            DrawRenderable(renderable, width, height, modelIndex);
        }
        else
        {
            for (auto &child : renderable->children)
            {
                DrawRenderable(child, width, height, modelIndex);
            }
        }

        modelIndex++;
    }

    // End renderpass and commandbuffer recording
    _renderPass->End(_commandBuffers[_currentFrame]);
    _commandBuffers[_currentFrame]->EndRecording();

    // Submit and present
    VkResult ret = this->SubmitAndPresent(currentCmdBuffer, imageIndex);

    // Increment current frame
    _currentFrame = (_currentFrame + 1) % MAX_CONCURRENT_FRAMES_IN_FLIGHT;
}

void Renderer::DrawRenderable(std::shared_ptr<Renderable> renderable, int width, int height, uint32_t modelIndex)
{

    // @todo refactors this

    auto translate = glm::translate(glm::mat4(1.0f), glm::vec3(renderable->position.x, renderable->position.y, renderable->position.z));

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

    modelMatrix = translate * scale * modelRotation;

    UboDataDynamic ubodyn{};

    if (renderable->type == 0 || renderable->type == 1)
    {
        ubodyn.view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        ubodyn.proj = glm::ortho(0.0f, (float)_swapChain->swapChainExtent.width, (float)_swapChain->swapChainExtent.height, 0.0f, -1.0f, 1.0f);
        // ubodyn.proj[1][1] *= -1;
    }
    else
    {
        ubodyn.view = _cam->GetViewMatrix();
        ubodyn.invView = glm::inverse(_cam->GetViewMatrix());
        ubodyn.proj = _camera->matrices.perspective;
        _camera->update(0.1);
    }

    ubodyn.model = translate * scale * modelRotation;
    ubodyn.color = renderable->color;
    ubodyn.colorBorder = renderable->colorBorder;
    ubodyn.colorShadow = renderable->colorShadow;
    ubodyn.position = renderable->position2d;
    ubodyn.size = renderable->scale;
    ubodyn.borderRadius = renderable->borderRadius;
    ubodyn.shapeId = renderable->type;

    uint32_t offset = dynamicAlignment * modelIndex;

    memcpy((char *)dynUbos[_currentFrame]->GetMappedMemory() + offset, &ubodyn, sizeof(UboDataDynamic));

    if (renderable->type == 1)
    {
        auto ds0 = _pipelines["Pipeline2D"]->descriptorSets[0]->Get()[_currentFrame];

        vkCmdBindPipeline(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelines["Pipeline2D"]->GetPipeline());
        vkCmdBindDescriptorSets(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelines["Pipeline2D"]->GetPipelineLayout(), 0, 1, &ds0, 1, &offset);
        auto sprite = std::dynamic_pointer_cast<Sprite>(renderable);
        vkCmdBindDescriptorSets(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelines["Pipeline2D"]->GetPipelineLayout(), 1, 1, &sprite->_descriptorSet, 0, nullptr);
    }

    if (renderable->type == 0)
    {
        auto ds0 = _pipelines["Pipeline2D"]->descriptorSets[0]->Get()[_currentFrame];

        vkCmdBindPipeline(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelines["Pipeline2D"]->GetPipeline());
        vkCmdBindDescriptorSets(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelines["Pipeline2D"]->GetPipelineLayout(), 0, 1, &ds0, 1, &offset);
    }

    if (renderable->type == 4)
    {
        auto ds0 = _pipelines["SkinnedPipeline"]->descriptorSets[0]->Get()[_currentFrame];
        vkCmdBindPipeline(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelines["SkinnedPipeline"]->GetPipeline());
        vkCmdBindDescriptorSets(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelines["SkinnedPipeline"]->GetPipelineLayout(), 0, 1, &ds0, 1, &offset);
        auto model = std::dynamic_pointer_cast<Model>(renderable);
        for (auto node : model->nodes)
        {
            model->renderNode(node, currentCmdBuffer, _pipelines["SkinnedPipeline"], Material::ALPHAMODE_OPAQUE);
        }
    }
    else if (renderable->type == 10)
    {
        /*
        vkCmdBindPipeline(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelines["CubeMapPipeline"]->GetPipeline());
        auto ds = _pipelines["CubeMapPipeline"]->descriptorSets[0]->Get()[_currentFrame];
        vkCmdBindDescriptorSets(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelines["CubeMapPipeline"]->GetPipelineLayout(), 0, 1, &currentDescriptorSet, 1, &offset);
        auto model = std::dynamic_pointer_cast<CubeMap>(renderable);
        model->_model->renderNode(model->_model->linearNodes[2], currentCmdBuffer, _pipelines["CubeMapPipeline"], Material::ALPHAMODE_OPAQUE);
        */
    }
    else
    {
        // Bind vertex & index buffers
        // Bind descriptor sets
        VkBuffer vertexBuffers[] = {renderable->vertexBuffer->GetVulkanBuffer()};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(currentCmdBuffer, 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(currentCmdBuffer, renderable->indexBuffer->GetVulkanBuffer(), 0, VK_INDEX_TYPE_UINT16);
        // Draw current renderable
        vkCmdDrawIndexed(currentCmdBuffer, renderable->GetIndices().size(), 1, 0, 0, 0);
    }

    FrameMark;
}

VkResult Renderer::SubmitAndPresent(VkCommandBuffer cmdBuffer, uint32_t imageIndex)
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
    return vkQueuePresentKHR(_logicalDevice->GetPresentQueue(), &presentInfo);
}
