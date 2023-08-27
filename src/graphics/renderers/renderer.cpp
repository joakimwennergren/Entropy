#include "renderer.hpp"

using namespace Symbios::Graphics::Renderers;

// Wrapper functions for aligned memory allocation
// There is currently no standard for this in C++ that works across all platforms and vendors, so we abstract this
// @todo put this in utils!!
void *alignedAlloc(size_t size, size_t alignment)
{
    void *data = nullptr;
#if defined(_MSC_VER) || defined(__MINGW32__)
    data = _aligned_malloc(size, alignment);
#else
    int res = posix_memalign(&data, alignment, size);
    if (res != 0)
        data = nullptr;
#endif
    return data;
}

void alignedFree(void *data)
{
#if defined(_MSC_VER) || defined(__MINGW32__)
    _aligned_free(data);
#else
    free(data);
#endif
}

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
    for (size_t i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++)
    {
        _uniformBuffers.push_back(new UniformBuffer(_context, sizeof(UniformBufferObject)));
    }

    /*
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    uniformBuffers.resize(MAX_CONCURRENT_FRAMES_IN_FLIGHT);
    uniformBuffersMemory.resize(MAX_CONCURRENT_FRAMES_IN_FLIGHT);
    uniformBuffersMapped.resize(MAX_CONCURRENT_FRAMES_IN_FLIGHT);


    for (size_t i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++)
    {
        Buffer::CreateBuffer(_context, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);

        vkMapMemory(_context->GetLogicalDevice(), uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
    }
    */

    // @todo this isnt necessary..
    _pipeline->Build();

    _quad = std::make_unique<Quad>(_context);
    _quad->position = glm::vec3(0.5, -0.5, 0.0);
    _quad->texture->CreateTextureImage("/Users/joakim/Desktop/Symbios/resources/textures/ivysaur.png");

    srand(static_cast<unsigned>(time(0)));

    /*
    for (int i = 0; i < 4; i++)
    {
        float x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        float y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        float z = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        auto glyph = new Quad(_context);
        glyph->texture->CreateTextureImage("/Users/joakim/Desktop/Symbios/resources/textures/ivysaur.png");
        glyph->position = glm::vec3(x, y * -1, 0.0);
        _text.push_back(glyph);
    }
    */

    // @temp dynamic UBO
    // Calculate required alignment based on minimum device offset alignment

    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(_context->GetPhysicalDevice(), &properties);
    size_t minUboAlignment = properties.limits.minUniformBufferOffsetAlignment;

    dynamicAlignment = sizeof(glm::mat4);

    if (minUboAlignment > 0)
    {
        dynamicAlignment = (dynamicAlignment + minUboAlignment - 1) & ~(minUboAlignment - 1);
    }

    // size_t bufferSizeUbo = 10 * dynamicAlignment;
    //_instanceUbo.model = (glm::mat4 *)alignedAlloc(bufferSizeUbo, dynamicAlignment);

    //_texture = std::make_unique<Texture>(_context);
    //_texture->CreateTextureImage("/Users/joakim/Desktop/Symbios/resources/textures/ivysaur.png");

    std::vector<VkBuffer> rawUniformBuffers;

    for (int i = 0; i < _uniformBuffers.size(); i++)
    {
        auto rawBuffer = _uniformBuffers[i]->GetVulkanBuffer();
        rawUniformBuffers.push_back(rawBuffer);
    }

    _context->CreateDescriptorSets(rawUniformBuffers, _quad->texture->GetImageView());

    hb_buffer_t *buf;
    buf = hb_buffer_create();
    hb_buffer_add_utf8(buf, "Test", -1, 0, -1);

    // If you know the direction, script, and language
    hb_buffer_set_direction(buf, HB_DIRECTION_LTR);
    hb_buffer_set_script(buf, HB_SCRIPT_LATIN);
    hb_buffer_set_language(buf, hb_language_from_string("en", -1));

    hb_blob_t *blob = hb_blob_create_from_file("/Users/joakim/Desktop/Symbios/resources/fonts/quick-kiss-font/QuickKissPersonalUse-PxlZ.ttf"); /* or hb_blob_create_from_file_or_fail() */
    hb_face_t *face = hb_face_create(blob, 0);
    hb_font_t *font = hb_font_create(face);

    hb_shape(font, buf, NULL, 0);
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

void Renderer::Render()
{
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

    auto currentDescriptorSet = _context->GetDescriptorSets()[_currentFrame];
    vkCmdBindDescriptorSets(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline->GetPipelineLayout(), 0, 1, &currentDescriptorSet, 0, nullptr);

    uint32_t modelCnt = 0;

    // One dynamic offset per dynamic descriptor to offset into the ubo containing all model matrices
    // uint32_t dynamicOffset = modelCnt * static_cast<uint32_t>(dynamicAlignment);
    // Bind the descriptor set for rendering a mesh using the dynamic offset

    // vkCmdBindDescriptorSets(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline->GetPipelineLayout(), 0, 1, &currentDescriptorSet, 1, &dynamicOffset);

    VkBuffer vertexBuffers[] = {_quad->vertexBuffer->GetVulkanBuffer()};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(currentCmdBuffer, 0, 1, vertexBuffers, offsets);

    vkCmdBindIndexBuffer(currentCmdBuffer, _quad->indexBuffer->GetVulkanBuffer(), 0, VK_INDEX_TYPE_UINT16);

    UniformBufferObject ubo{};
    ubo.model = glm::mat4(1.0f);
    ubo.model = glm::translate(glm::mat4(1.0), _quad->position);
    ubo.model = glm::scale(ubo.model, glm::vec3(0.25, 0.25, 0.25));

    ubo.view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    float aspect = (float)_context->GetSwapChainExtent().width / (float)_context->GetSwapChainExtent().height;
    ubo.proj = glm::ortho(0.0f, aspect, 0.0f, 1.0f, 0.0f, 100.0f);
    ubo.proj[1][1] *= -1;

    memcpy(_uniformBuffers[_currentFrame]->GetMappedMemory(), &ubo, sizeof(ubo));

    vkCmdDrawIndexed(currentCmdBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

    modelCnt++;

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

void Renderer::UpdateUniforms(uint32_t currentImage)
{
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo{};
    ubo.model = glm::mat4(1.0f);
    ubo.model = glm::translate(glm::mat4(1.0), glm::vec3(0.5, -0.5, 0.0));
    ubo.model = glm::scale(ubo.model, glm::vec3(0.25, 0.25, 0.25));

    ubo.view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    float aspect = (float)_context->GetSwapChainExtent().width / (float)_context->GetSwapChainExtent().height;
    ubo.proj = glm::ortho(0.0f, aspect, 0.0f, 1.0f, 0.0f, 100.0f);
    ubo.proj[1][1] *= -1;

    memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}
