#include "vulkan_renderer.hpp"
#include "data/pushcontants.hpp"
#include "data/ubo.hpp"
#include "ecs/components/animated_sprite.hpp"
#include "ecs/components/color.hpp"
#include "ecs/components/hasTexture.hpp"
#include "ecs/components/position.hpp"
#include "ecs/components/renderable.hpp"
#include "ecs/components/rotation.hpp"
#include "ecs/components/scale.hpp"
#include "vulkan/vulkan_core.h"

using namespace Entropy::Graphics::Vulkan::Renderers;

void VulkanRenderer::Render(int width, int height,
                            bool &needResize) {
    if (_world->Get()->count<Components::Renderable>() == 0)
        return;


    const auto currentFence = _synchronizer->GetFences()[_currentFrame];

    vkWaitForFences(_logicalDevice->Get(), 1, &currentFence, VK_TRUE,
                    UINT64_MAX);

    vkResetFences(_logicalDevice->Get(), 1, &currentFence);

    vkAcquireNextImageKHR(_logicalDevice->Get(), _swapchain->Get(), UINT64_MAX,
                          _synchronizer->GetImageSemaphores()[_currentFrame],
                          VK_NULL_HANDLE, &imageIndex);


    if (needResize) {
        stagingBuffer = std::make_shared<StagingBuffer>(
            width * height * 4, nullptr, VK_BUFFER_USAGE_TRANSFER_DST_BIT);
        _synchronizer =
                std::make_unique<Synchronizer>(MAX_CONCURRENT_FRAMES_IN_FLIGHT);
        _swapchain->RecreateSwapChain(width, height);
        _renderPass->RecreateDepthBuffer(width, height);
        _renderPass->RecreateFrameBuffers(width, height);
        needResize = false;
        return;
    }

    const auto orthoCamera =
            dynamic_cast<OrthographicCamera *>(_cameraManager->currentCamera);
    orthoCamera->setPerspective((float) width, (float) height, 0.1f, 256.0f);

    // Begin render pass and command buffer recording
    _commandBuffers[_currentFrame].Record();
    _renderPass->Begin(_commandBuffers[_currentFrame],
                       imageIndex, width,
                       height);

    UboDataDynamic ubodyn{};
    ubodyn.perspective = orthoCamera->matrices.perspective;
    ubodyn.view = orthoCamera->matrices.view;
    memcpy(_UBO->GetMappedMemory(), &ubodyn, sizeof(ubodyn));

    _sortQuery.each([this, width, height](const flecs::entity e,
                                          Components::Position p) {
        auto scale_component = e.get_ref<Components::Scale>();
        auto color_component = e.get_ref<Components::Color>();
        auto rotation_component = e.get_ref<Components::Rotation>();
        auto render_component = e.get_ref<Components::Renderable>();

        auto rotation = glm::mat4(1.0f);
        auto scaling = glm::mat4(1.0f);

        const auto translate = glm::translate(glm::mat4(1.0f), p.pos);

        if (scale_component.get() != nullptr) {
            scaling = scale(glm::mat4(1.0f), scale_component->scale);
        }

        if (rotation_component.get() != nullptr) {
            rotation =
                    rotate(glm::mat4(1.0f), glm::radians(rotation_component->angle),
                           rotation_component->orientation);
        }

        void *objectData;
        vmaMapMemory(_allocator->Get(), _instanceDataBuffer->_allocation,
                     &objectData);

        auto *objectSSBO = static_cast<InstanceData *>(objectData);
        objectSSBO[render_component->id - 1].model =
                (translate * scaling * rotation);

        objectSSBO[render_component->id - 1].color =
                color_component.get() == nullptr
                    ? glm::vec4(1.0, 1.0, 1.0, 1.0)
                    : color_component->color;

        objectSSBO[render_component->id - 1].type = render_component->type;
        objectSSBO[render_component->id - 1].resolution =
                glm::vec2{(float) width, (float) height};

        vmaUnmapMemory(_allocator->Get(), _instanceDataBuffer->_allocation);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = {(unsigned int) width, (unsigned int) height};
        vkCmdSetScissor(_commandBuffers[_currentFrame].Get(), 0, 1, &scissor);

        // Set Viewport
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float) width;
        viewport.height = (float) height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(_commandBuffers[_currentFrame].Get(), 0, 1, &viewport);

        vkCmdBindPipeline(_commandBuffers[_currentFrame].Get(),
                          VK_PIPELINE_BIND_POINT_GRAPHICS,
                          _staticPipeline->GetPipeline());

        const auto ds0 = _staticPipeline->descriptorSets[0].Get()[_currentFrame];

        vkCmdBindDescriptorSets(
            _commandBuffers[_currentFrame].Get(), VK_PIPELINE_BIND_POINT_GRAPHICS,
            _staticPipeline->GetPipelineLayout(), 0, 1, &ds0, 0, nullptr);

        if (e.has<Components::HasTexture>()) {
            const auto texture = e.get<Components::HasTexture>();
            vkCmdBindDescriptorSets(_commandBuffers[_currentFrame].Get(),
                                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                                    _staticPipeline->GetPipelineLayout(), 1, 1,
                                    &texture->texture->descriptorSet, 0, nullptr);
        }

        if (e.has<Components::HasAnimatedSprite>()) {
            auto textures = e.get<Components::HasAnimatedSprite>();
            static int textureId;

            if (_timer->GetTick() >= 120.0) {
                textureId = (textureId + 1) % textures->textures.size();
                _timer->Reset();
            }

            vkCmdBindDescriptorSets(
                _commandBuffers[_currentFrame].Get(), VK_PIPELINE_BIND_POINT_GRAPHICS,
                _staticPipeline->GetPipelineLayout(), 1, 1,
                &textures->textures[textureId]->descriptorSet, 0, nullptr);
        }

        PushConstBlock constants{};
        constants.instanceIndex = render_component->id - 1;
        constants.hasTexture = 0;

        // upload the matrix to the GPU via push constants
        vkCmdPushConstants(_commandBuffers[_currentFrame].Get(),
                           _staticPipeline->GetPipelineLayout(),
                           VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstBlock),
                           &constants);

        if (render_component.get() != nullptr &&
            e.get<Entropy::Components::Renderable>()->indexBuffer == nullptr) {
            auto renderable = e.get<Entropy::Components::Renderable>();
            // Bind vertex & index buffers
            VkBuffer vertexBuffers[] = {renderable->vertexBuffer->GetVulkanBuffer()};
            VkDeviceSize offsets[] = {0};
            vkCmdBindVertexBuffers(_commandBuffers[_currentFrame].Get(), 0, 1,
                                   vertexBuffers, offsets);

            vkCmdDraw(_commandBuffers[_currentFrame].Get(),
                      renderable->vertices.size(), 1, 0, 0);
        }

        if (render_component.get() != nullptr &&
            e.get<Components::Renderable>()->indexBuffer != nullptr) {
            const auto renderable = e.get<Components::Renderable>();
            // Bind vertex & index buffers
            const VkBuffer vertexBuffers[] = {renderable->vertexBuffer->GetVulkanBuffer()};
            VkDeviceSize offsets[] = {0};
            vkCmdBindVertexBuffers(_commandBuffers[_currentFrame].Get(), 0, 1,
                                   vertexBuffers, offsets);

            vkCmdBindIndexBuffer(_commandBuffers[_currentFrame].Get(),
                                 renderable->indexBuffer->GetVulkanBuffer(), 0,
                                 VK_INDEX_TYPE_UINT16);

            // Draw current renderable
            vkCmdDrawIndexed(_commandBuffers[_currentFrame].Get(),
                             renderable->indices.size(), 1, 0, 0, 0);
        }
    });

    PresentForApplication();

    _currentFrame = (_currentFrame + 1) % MAX_CONCURRENT_FRAMES_IN_FLIGHT;
}
