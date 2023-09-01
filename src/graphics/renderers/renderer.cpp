#include "renderer.hpp"

using namespace Symbios::Graphics::Renderers;

std::vector<Quad *> Renderer::RenderText(std::string text, float x, float y, float scale, glm::vec3 color)
{
    // iterate through all characters

    std::string::const_iterator c;

    std::vector<Quad *> glyphs;

    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        ch.glyph->position = glm::vec3(x, y, 0.0);

        glyphs.push_back(ch.glyph);

        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }

    return glyphs;
}

Renderer::Renderer(std::shared_ptr<Context> context)
{
    // Store ctx
    _context = context;

    // Create renderpass
    _renderPass = std::make_shared<RenderPass>(_context);

    // Create pipeline(s)
    _pipeline = std::make_unique<Pipeline>(_context, _renderPass);

    // @todo this isnt necessary..
    _pipeline->Build();

    for (uint32_t i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++)
    {
        _commandBuffers.push_back(std::make_shared<CommandBuffer>(_context));
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
        _uniformBuffers.push_back(new UniformBuffer(_context, sizeof(UniformBufferObject)));
    }

    ivy7 = new Quad(_context);
    ivy7->position = glm::vec3(500.0, -500.0, 0.0);
    ivy7->scale = glm::vec3(100.0, 100.0, 0.0);
    ivy7->textureId = 2;
    ivy7->texture->CreateTextureImage("/Users/joakim/Desktop/Symbios/resources/textures/svamp.png");

    auto L = Singleton::GetInstance("tset")->GetState();

    getGlobalNamespace(L)
        .beginClass<Context>("Context")
        .addConstructor<void (*)()>()
        .endClass();

    getGlobalNamespace(L)
        .beginClass<Quad>("Quad")
        .addConstructor<void (*)(void)>()
        .addFunction("println", &Quad::Test)
        .addFunction("setPosition", &Quad::SetPosition)
        .endClass();

    luabridge::setGlobal(L, _context.get(), "Context");

    //_sprites.push_back(ivy7);

    srand(static_cast<unsigned>(time(0)));

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

    // every item in descriptorImageInfos needs an imageview...

    /*
    hb_buffer_t *buf;
    buf = hb_buffer_create();
    hb_buffer_add_utf8(buf, "Test", -1, 0, -1);

    // If you know the direction, script, and language
    hb_buffer_set_direction(buf, HB_DIRECTION_LTR);
    hb_buffer_set_script(buf, HB_SCRIPT_LATIN);
    hb_buffer_set_language(buf, hb_language_from_string("en", -1));

    hb_blob_t *blob = hb_blob_create_from_file("/Users/joakim/Desktop/Symbios/resources/fonts/quick-kiss-font/QuickKissPersonalUse-PxlZ.ttf");
    hb_face_t *face = hb_face_create(blob, 0);
    hb_font_t *font = hb_font_create(face);

    hb_shape(font, buf, NULL, 0);

    glyph_info = hb_buffer_get_glyph_infos(buf, &glyph_count);
    glyph_pos = hb_buffer_get_glyph_positions(buf, &glyph_count);

    hb_position_t cursor_x = 0;
    hb_position_t cursor_y = 0;
    for (unsigned int i = 0; i < glyph_count; i++)
    {
        hb_codepoint_t glyphid = glyph_info[i].codepoint;
        hb_position_t x_offset = glyph_pos[i].x_offset;
        hb_position_t y_offset = glyph_pos[i].y_offset;
        hb_position_t x_advance = glyph_pos[i].x_advance;
        hb_position_t y_advance = glyph_pos[i].y_advance;
        draw_glyph(glyphid, cursor_x + x_offset, cursor_y + y_offset);
        cursor_x += x_advance;
        cursor_y += y_advance;
    }
    */

    FT_Library ft;
    FT_Face face;

    FT_Init_FreeType(&ft);

    FT_New_Face(ft, "/Users/joakim/Desktop/Symbios/resources/fonts/lato/Lato-Regular.ttf", 0, &face);

    FT_Set_Pixel_Sizes(face, 0, 64);

    for (uint8_t c = 32; c < 128; c++)
    {
        auto glyph_index = FT_Get_Char_Index(face, c);

        if (glyph_index != 0)
        {
            auto error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);

            if (error)
            {
                throw std::runtime_error("failed to load glyph");
            }

            FT_GlyphSlot glyphSlot = face->glyph;

            error = FT_Render_Glyph(glyphSlot, FT_RENDER_MODE_NORMAL);
            if (error)
            {
                throw std::runtime_error("failed to render glyph");
            }

            if (glyphSlot->bitmap.width != 0)
            {
                auto g = new Quad(_context);
                g->position = glm::vec3(500.0, -500.0, 0.0);
                g->textureId = 2;
                g->texture->CreateTextureImageFromBuffer(face->glyph->bitmap);

                Character character = {
                    g,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    face->glyph->advance.x};

                //_sprites.push_back(g);

                Characters.insert(std::pair<char, Character>(c, character));
            }
        }
    }

    //  g->UpdateImage();

    // generate texture
    /*
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        face->glyph->bitmap.width,
        face->glyph->bitmap.rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        face->glyph->bitmap.buffer);
    // set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // now store character for later use
    Character character = {
        texture,
        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        face->glyph->advance.x};
    Characters.insert(std::pair<char, Character>(c, character));
    */
    std::string text = "TEST";

    float x = 200.0, y = -500.0;
    float scale = 0.8;
    std::string::const_iterator c;

    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x;
        float ypos = y - (ch.Size.y - ch.Bearing.y);

        float w = ch.Size.x;
        float h = ch.Size.y;

        auto g = new Quad(_context);
        g->position = glm::vec3(xpos, ypos, 0.0);
        g->textureId = 2;
        g->scale = glm::vec3(w, h, 0.0);
        g->texture = ch.glyph->texture;

        _sprites.push_back(g);

        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6); // bitshift by 6 to get value in pixels (2^6 = 64)
    }

    for (auto sprite : _sprites)
    {
        if (sprite->texture->hasTexture)
            sprite->UpdateImage();
    }

    _context->CreateDescriptorSets(rawUniformBuffers, ivy7->texture->GetImageView());

    luaL_dofile(Singleton::GetInstance("test")->GetState(), "/Users/joakim/Desktop/Symbios/resources/scripts/test.lua");
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

    float scale = 0.1;

    // scale += glfwGetTime() * 0.01;
    // ivy7->scale = glm::vec3(scale, scale, 0.0);

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

    for (auto sprite : _sprites)
    {
        // One dynamic offset per dynamic descriptor to offset into the ubo containing all model matrices
        // uint32_t dynamicOffset = modelCnt * static_cast<uint32_t>(dynamicAlignment);
        // Bind the descriptor set for rendering a mesh using the dynamic offset

        // vkCmdBindDescriptorSets(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline->GetPipelineLayout(), 0, 1, &currentDescriptorSet, 1, &dynamicOffset);

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

        InstancePushConstants constants;
        constants.modelMatrix = translate * scale * rotate;
        constants.color = sprite->color;

        if (sprite->texture->GetImageView() != nullptr)
        {
            constants.textureId = sprite->textureId;
        }

        // PLOG_ERROR << (float)_context->GetSwapChainExtent().height;

        // upload the matrix to the GPU via push constants
        vkCmdPushConstants(currentCmdBuffer, _pipeline->GetPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(InstancePushConstants), &constants);

        ubo.view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        float V = (float)_context->GetSwapChainExtent().width / (float)_context->GetSwapChainExtent().height;

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
