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

void Renderer::DrawGUI()
{
    ImDrawData *imDrawData = ImGui::GetDrawData();
    ImGuiIO &io = ImGui::GetIO();

    // Note: Alignment is done inside buffer creation
    VkDeviceSize vertexBufferSize = imDrawData->TotalVtxCount;
    VkDeviceSize indexBufferSize = imDrawData->TotalIdxCount;

    if ((vertexBufferSize == 0) || (indexBufferSize == 0) || imDrawData->CmdListsCount == 0)
    {
        return;
    }

    // Vertex buffer

    _vertexBuffer.reset();

    // VkDeviceMemory stagingBufferMemory;
    global_vtx_offset = 0;
    std::vector<Vertex> vertices(imDrawData->TotalVtxCount);
    for (int n = 0; n < imDrawData->CmdListsCount; n++)
    {
        const ImDrawList *cmd_list = imDrawData->CmdLists[n];
        for (int i = 0; i < cmd_list->VtxBuffer.size(); i++)
        {
            vertices[global_vtx_offset + i].position = glm::vec3(cmd_list->VtxBuffer.Data[i].pos.x, cmd_list->VtxBuffer.Data[i].pos.y, 0.0);
            vertices[global_vtx_offset + i].uv0 = glm::vec2(cmd_list->VtxBuffer.Data[i].uv.x, cmd_list->VtxBuffer.Data[i].uv.y);
            auto color = glm::vec4(
                (cmd_list->VtxBuffer.Data[i].col & 0x000000FF) / 255.0,
                (cmd_list->VtxBuffer.Data[i].col & 0x0000FF00) / 65280.0,
                (cmd_list->VtxBuffer.Data[i].col & 0x00FF0000) / 16711680.0,
                (cmd_list->VtxBuffer.Data[i].col & 0xFF000000) / 4278190080.0);
            vertices[global_vtx_offset + i].color = color;
        }
        global_vtx_offset += cmd_list->VtxBuffer.size();
    }
    _vertexBuffer = std::make_unique<VertexBuffer>(_serviceLocator, vertices);

    global_idx_offset = 0;
    std::vector<uint16_t> indices(imDrawData->TotalIdxCount);
    for (int n = 0; n < imDrawData->CmdListsCount; n++)
    {
        const ImDrawList *cmd_list = imDrawData->CmdLists[n];
        for (int i = 0; i < cmd_list->IdxBuffer.size(); i++)
        {
            indices[global_idx_offset + i] = cmd_list->IdxBuffer.Data[i];
        }
        global_idx_offset += cmd_list->IdxBuffer.size();
    }
    _indexBuffer = std::make_unique<Buffer>();
    _indexBuffer->CreateIndexBufferUint16(_serviceLocator, indices);


    PushConstBlock pushConstBlock{};
    // UI scale and translate via push constants
    pushConstBlock.scale = glm::vec2(2.0f / io.DisplaySize.x, 2.0f / io.DisplaySize.y);
    pushConstBlock.translate = glm::vec2(-1.0f);
    vkCmdPushConstants(currentCmdBuffer, _pipelines["GUIPipeline"]->GetPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstBlock), &pushConstBlock);

    auto ds0 = _pipelines["GUIPipeline"]->descriptorSets[0]->Get()[_currentFrame];
    vkCmdBindPipeline(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelines["GUIPipeline"]->GetPipeline());
    vkCmdBindDescriptorSets(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelines["GUIPipeline"]->GetPipelineLayout(), 0, 1, &ds0, 0, nullptr);
    VkViewport viewport{};
    viewport.width = ImGui::GetIO().DisplaySize.x;
    viewport.height = ImGui::GetIO().DisplaySize.y;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(currentCmdBuffer, 0, 1, &viewport);

    if (imDrawData->CmdListsCount > 0)
    {

        VkDeviceSize offsets[1] = {0};
        auto indexBuff = _indexBuffer->GetVulkanBuffer();
        auto v = _vertexBuffer->GetVulkanBuffer();

        global_idx_offset = 0;
        global_vtx_offset = 0;
        vkCmdBindVertexBuffers(currentCmdBuffer, 0, 1, &v, offsets);
        vkCmdBindIndexBuffer(currentCmdBuffer, indexBuff, 0, VK_INDEX_TYPE_UINT16);

        for (int32_t i = 0; i < imDrawData->CmdListsCount; i++)
        {
            const ImDrawList *cmd_list = imDrawData->CmdLists[i];

            for (int32_t j = 0; j < cmd_list->CmdBuffer.Size; j++)
            {
                const ImDrawCmd *pcmd = &cmd_list->CmdBuffer[j];
                VkRect2D scissorRect;
                scissorRect.offset.x = std::max((int32_t)(pcmd->ClipRect.x), 0);
                scissorRect.offset.y = std::max((int32_t)(pcmd->ClipRect.y), 0);
                scissorRect.extent.width = (uint32_t)(pcmd->ClipRect.z - pcmd->ClipRect.x);
                scissorRect.extent.height = (uint32_t)(pcmd->ClipRect.w - pcmd->ClipRect.y);
                vkCmdSetScissor(currentCmdBuffer, 0, 1, &scissorRect);
                vkCmdDrawIndexed(currentCmdBuffer, pcmd->ElemCount, 1, pcmd->IdxOffset + global_idx_offset, global_vtx_offset, 0);
            }
            global_idx_offset += cmd_list->IdxBuffer.Size;
            global_vtx_offset += cmd_list->VtxBuffer.Size;
        }
    }
}

void Renderer::Setup(std::shared_ptr<ServiceLocator> serviceLocator, float xscale, float yscale)
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
    _synchronizer = std::make_shared<Synchronizer>(MAX_CONCURRENT_FRAMES_IN_FLIGHT, serviceLocator);

    for (uint32_t i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++)
    {
        _commandBuffers.push_back(std::make_shared<CommandBuffer>(serviceLocator, VK_COMMAND_BUFFER_LEVEL_PRIMARY));
    }

    // Create buffers @todo temp!!!
    for (size_t i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++)
    {
        _uniformBuffers.push_back(new UniformBuffer(serviceLocator, sizeof(UniformBufferObject)));
        dynUbos.push_back(new UniformBuffer(serviceLocator, sizeof(UboDataDynamic) * 10000));
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
        std::array<VkWriteDescriptorSet, 6> descriptorWrites{};

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
        descriptorWrites[2].dstSet = _pipelines["GizmoPipeline"]->descriptorSets[0]->Get()[i];
        descriptorWrites[2].dstBinding = 0;
        descriptorWrites[2].dstArrayElement = 0;
        descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[2].descriptorCount = 1;
        descriptorWrites[2].pBufferInfo = &bufferInfo;

        descriptorWrites[3].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[3].dstSet = _pipelines["SkinnedPipeline"]->descriptorSets[0]->Get()[i];
        descriptorWrites[3].dstBinding = 1;
        descriptorWrites[3].dstArrayElement = 0;
        descriptorWrites[3].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        descriptorWrites[3].descriptorCount = 1;
        descriptorWrites[3].pBufferInfo = &bufferInfo2;

        descriptorWrites[4].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[4].dstSet = _pipelines["Pipeline2D"]->descriptorSets[0]->Get()[i];
        descriptorWrites[4].dstBinding = 1;
        descriptorWrites[4].dstArrayElement = 0;
        descriptorWrites[4].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        descriptorWrites[4].descriptorCount = 1;
        descriptorWrites[4].pBufferInfo = &bufferInfo2;

        descriptorWrites[5].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[5].dstSet = _pipelines["GizmoPipeline"]->descriptorSets[0]->Get()[i];
        descriptorWrites[5].dstBinding = 1;
        descriptorWrites[5].dstArrayElement = 0;
        descriptorWrites[5].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        descriptorWrites[5].descriptorCount = 1;
        descriptorWrites[5].pBufferInfo = &bufferInfo2;

        vkUpdateDescriptorSets(_logicalDevice->Get(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }

    _camera = std::make_shared<Camera>();
    _camera->type = Camera::CameraType::firstperson;
    _camera->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    _camera->setRotation(glm::vec3(0.0f));

    // ********* START IMGUI STUFF ***********

    // Setup Dear ImGui context

    ImGuiIO &io = ImGui::GetIO();
    // io.Fonts->AddFontFromFileTTF("/Users/joakim/Entropy-Engine/resources/fonts/Roboto-2/Roboto-Regular.ttf", 16);
    //  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    //  Setup Dear ImGui style
    ImGui::StyleColorsDark();
    io.Fonts->AddFontFromFileTTF("/Users/joakim/Entropy-Engine/resources/fonts/lato/Lato-Regular.ttf", 16 * xscale);
    unsigned char *fontData;
    int texWidth, texHeight;
    io.Fonts->GetTexDataAsRGBA32(&fontData, &texWidth, &texHeight);

    auto texture = new Texture(_serviceLocator);
    texture->CreateTextureImageFromPixels(fontData, texWidth, texHeight);

    VkSampler _sampler;

    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(physicalDevice->Get(), &properties);

    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;

    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;

    if (vkCreateSampler(_logicalDevice->Get(), &samplerInfo, nullptr, &_sampler) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create texture sampler!");
    }

    for (int i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++)
    {
        VkDescriptorImageInfo descriptorImageInfo{};
        descriptorImageInfo.sampler = _sampler;
        descriptorImageInfo.imageView = texture->GetImageView();
        descriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        std::array<VkWriteDescriptorSet, 1> descriptorWrites{};

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = _pipelines["GUIPipeline"]->descriptorSets[0]->Get()[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pImageInfo = &descriptorImageInfo;

        vkUpdateDescriptorSets(_logicalDevice->Get(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
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
/*
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
    */
}
#endif
Renderer::Renderer(std::shared_ptr<ServiceLocator> serviceLocator, float xscale, float yscale)
{
    // Create renderpass
    _renderPass = std::make_shared<RenderPass>(serviceLocator);
    // Create skinned pipeline
    _pipelines["SkinnedPipeline"] = std::make_shared<SkinnedPipeline>(_renderPass, serviceLocator, VK_POLYGON_MODE_FILL);
    _pipelines["CubeMapPipeline"] = std::make_shared<CubeMapPipeline>(_renderPass, serviceLocator, VK_POLYGON_MODE_FILL);
    _pipelines["Pipeline2D"] = std::make_shared<Pipeline2D>(_renderPass, serviceLocator, VK_POLYGON_MODE_FILL);
    _pipelines["GUIPipeline"] = std::make_shared<GUIPipeline>(_renderPass, serviceLocator, VK_POLYGON_MODE_FILL);
    _pipelines["GizmoPipeline"] = std::make_shared<GizmoPipeline>(_renderPass, serviceLocator, VK_POLYGON_MODE_FILL);
    Setup(serviceLocator, xscale, yscale);
}

void Renderer::HandleResize()
{
    ZoneScopedN("Resizing");

    _synchronizer.reset();
    _synchronizer = std::make_unique<Synchronizer>(MAX_CONCURRENT_FRAMES_IN_FLIGHT, _serviceLocator);
    _swapChain->RecreateSwapChain();
    _renderPass->RecreateDepthBuffer();
    _renderPass->RecreateFrameBuffers();
}

void Renderer::Wireframe(bool on)
{
        _pipelines["SkinnedPipeline"] = std::make_shared<SkinnedPipeline>(_renderPass, _serviceLocator, on == true ? VK_POLYGON_MODE_LINE : VK_POLYGON_MODE_FILL);

            for (size_t i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++)
    {
        std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

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
        descriptorWrites[1].dstSet = _pipelines["SkinnedPipeline"]->descriptorSets[0]->Get()[i];
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pBufferInfo = &bufferInfo2;


        vkUpdateDescriptorSets(_logicalDevice->Get(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
}

VkResult Renderer::DoRender(int width, int height)
{

    VkResult submit_result = VK_SUCCESS;

    _camera->setPerspective(60.0f, (float)width / (float)height, 0.1f, 256.0f);

    // Reset fences and current commandbuffer
    vkResetFences(_logicalDevice->Get(), 1, &_synchronizer->GetFences()[_currentFrame]);

    // current commandbuffer & descriptorset
    currentCmdBuffer = _commandBuffers[_currentFrame]->GetCommandBuffer();

    vkResetCommandBuffer(currentCmdBuffer, 0);

    /*
    if (cmdBufferUI != nullptr)
    {
        auto secondaries = cmdBufferUI->GetCommandBuffer();
        vkCmdExecuteCommands(
            currentCmdBuffer,
            1,
            &secondaries);
    }
    */

    // Begin renderpass and commandbuffer recording
    _commandBuffers[_currentFrame]->Record();
    _renderPass->Begin(_commandBuffers[_currentFrame], imageIndex);

    // Sort renderables based on Zindex
    sort(_sceneGraph->renderables.begin(),
         _sceneGraph->renderables.end(),
         [](const std::shared_ptr<Renderable> &lhs, const std::shared_ptr<Renderable> &rhs)
         { return lhs->zIndex < rhs->zIndex; });

    UniformBufferObject ubo{};

    ubo.screen = glm::vec2(500.0, 500.0);

    ubo.view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    ubo.proj = glm::ortho(0.0f, (float)_swapChain->swapChainExtent.width, (float)_swapChain->swapChainExtent.height, 0.0f, -1.0f, 1.0f);
    ubo.proj[1][1] *= -1;

    uint32_t modelIndex = 0;

    for (auto &renderable : _sceneGraph->renderables)
    {

        float offsetX = 0.0;

        if (renderable->type == 4)
        {
            offsetX = (48.0 * 2.0) + ((width * 0.20) - (48.0 / 2.0));
        }

        if (renderable->overflowHidden)
        {
            // Scissor
            VkRect2D scissor{};
            scissor.offset = {(int32_t)renderable->position.x, (int32_t)renderable->position.y};
            scissor.extent = {(uint32_t)renderable->scale.x, (uint32_t)renderable->scale.y};
            vkCmdSetScissor(currentCmdBuffer, 0, 1, &scissor);
        }
        else
        {
            VkRect2D scissor{};
            scissor.offset = {0, 0};
            scissor.extent = {_swapChain->swapChainExtent.width, _swapChain->swapChainExtent.height};
            vkCmdSetScissor(currentCmdBuffer, 0, 1, &scissor);
        }

        // Set Viewport
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)_swapChain->swapChainExtent.width;
        viewport.height = (float)_swapChain->swapChainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(currentCmdBuffer, 0, 1, &viewport);

        renderable->Test();
        // Update UBO

        // ubo.screen = glm::vec2((float)width, (float)height);

        memcpy(_uniformBuffers[_currentFrame]->GetMappedMemory(), &ubo, sizeof(ubo));

        if (renderable->children.size() == 0)
        {
            DrawRenderable(renderable, 500.0, 500.0, modelIndex);
        }
        else
        {
            uint32_t childIndex = modelIndex;

            for (auto &child : renderable->children)
            {
                DrawRenderable(child, 500.0, 500.0, childIndex * modelIndex);
                childIndex++;
            }
        }

        modelIndex++;
    }


    DrawGUI();

    // End renderpass and commandbuffer recording
    _renderPass->End(_commandBuffers[_currentFrame]);
    _commandBuffers[_currentFrame]->EndRecording();

    // Submit and present
    submit_result = this->SubmitAndPresent(currentCmdBuffer, imageIndex);

    _currentFrame = (_currentFrame + 1) % MAX_CONCURRENT_FRAMES_IN_FLIGHT;
    return submit_result;
}

void Renderer::Render(int width, int height, bool resize)
{
    vkWaitForFences(_logicalDevice->Get(), 1, &_synchronizer->GetFences()[_currentFrame], VK_TRUE, UINT64_MAX);

    auto acquire_result = vkAcquireNextImageKHR(_logicalDevice->Get(), _swapChain->Get(), UINT64_MAX, _synchronizer->GetImageSemaphores()[_currentFrame], VK_NULL_HANDLE, &imageIndex);

    VkResult submit_result = VK_SUCCESS;

    if (acquire_result == VK_SUCCESS || acquire_result == VK_SUBOPTIMAL_KHR)
    {
        // std::cout << "Rendering" << std::endl;
        submit_result = DoRender(width, height);
    }

    if (acquire_result != VK_SUCCESS || submit_result != VK_SUCCESS)
    {
        // std::cout << "RESIZING.." << std::endl;
        HandleResize();
    }
}

void Renderer::DrawRenderable(std::shared_ptr<Renderable> renderable, int width, int height, uint32_t modelIndex)
{
    if(!renderable->visible)
        return;

    // @todo refactors this

    auto translate = glm::translate(glm::mat4(1.0f), glm::vec3(renderable->position.x, renderable->position.y, renderable->position.z));

    auto scale = glm::scale(glm::mat4(1.0f), renderable->scale);

    auto rotate = glm::mat4(1.0);

    if (renderable->type == 4)
    {
    }
    else
    {
        rotate = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
    }

    auto modelRotation = glm::rotate(glm::mat4(1.0f), glm::radians(renderable->rotation), renderable->orientation);
    // @todo end

    // if (renderable->texture->GetImageView() != nullptr)
    // {

    // }

    UboDataDynamic ubodyn{};

    if (renderable->type == 0 || renderable->type == 1 || renderable->type == 3)
    {
        ubodyn.view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        ubodyn.proj = glm::ortho(0.0f, (float)_swapChain->swapChainExtent.width, (float)_swapChain->swapChainExtent.height, 0.0f, -1.0f, 1.0f);
        ubodyn.proj[1][1] *= -1;
    }
    else
    {
        ubodyn.view = _cam->GetViewMatrix();
        ubodyn.invView = glm::inverse(_cam->GetViewMatrix());
        ubodyn.proj = _camera->matrices.perspective;
        _camera->update(0.1);
    }

    ubodyn.model = translate * modelRotation * scale;
    ubodyn.color = renderable->color;
    ubodyn.colorBorder = renderable->colorBorder;
    ubodyn.colorShadow = renderable->colorShadow;
    ubodyn.position = renderable->position2d;
    ubodyn.size = renderable->scale;
    ubodyn.borderRadius = renderable->borderRadius;
    ubodyn.shapeId = renderable->type;

    uint32_t offset = dynamicAlignment * modelIndex;

    memcpy((char *)dynUbos[_currentFrame]->GetMappedMemory() + offset, &ubodyn, sizeof(UboDataDynamic));

    if (renderable->type == 1 || renderable->type == 3)
    {
        auto ds0 = _pipelines["Pipeline2D"]->descriptorSets[0]->Get()[_currentFrame];

        vkCmdBindPipeline(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelines["Pipeline2D"]->GetPipeline());
        vkCmdBindDescriptorSets(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelines["Pipeline2D"]->GetPipelineLayout(), 0, 1, &ds0, 1, &offset);
        auto sprite = std::dynamic_pointer_cast<Sprite>(renderable);
        vkCmdBindDescriptorSets(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelines["Pipeline2D"]->GetPipelineLayout(), 1, 1, &sprite->_descriptorSet, 0, nullptr);
    }

    if (renderable->type == 5)
    {
        auto ds0 = _pipelines["GizmoPipeline"]->descriptorSets[0]->Get()[_currentFrame];
        vkCmdBindPipeline(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelines["GizmoPipeline"]->GetPipeline());
        vkCmdBindDescriptorSets(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelines["GizmoPipeline"]->GetPipelineLayout(), 0, 1, &ds0, 1, &offset);

        VkBuffer vertexBuffers[] = {renderable->vertexBuffer->GetVulkanBuffer()};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(currentCmdBuffer, 0, 1, vertexBuffers, offsets);
        vkCmdDraw(currentCmdBuffer, 6, 1, 0, 0);
        return;

    }

    if (renderable->type == 0)
    {
        auto ds0 = _pipelines["Pipeline2D"]->descriptorSets[0]->Get()[_currentFrame];

        vkCmdBindPipeline(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelines["Pipeline2D"]->GetPipeline());
        vkCmdBindDescriptorSets(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelines["Pipeline2D"]->GetPipelineLayout(), 0, 1, &ds0, 1, &offset);
        auto sprite = std::dynamic_pointer_cast<Quad>(renderable);
        vkCmdBindDescriptorSets(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelines["Pipeline2D"]->GetPipelineLayout(), 1, 1, &sprite->_descriptorSet, 0, nullptr);
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
