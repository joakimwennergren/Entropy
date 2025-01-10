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

using namespace Entropy::Graphics::Vulkan::Renderers;

void VulkanRenderer::Render(const int width, const int height,
                            const float xscale, const float yscale) {
    if (_world->Get()->count<Components::Renderable>() == 0)
        return;

    const auto currentFence = _synchronizer->GetFences()[_currentFrame];

    vkWaitForFences(_logicalDevice->Get(), 1, &currentFence, VK_TRUE,
                    UINT64_MAX);

    vkResetFences(_logicalDevice->Get(), 1, &currentFence);

    vkAcquireNextImageKHR(_logicalDevice->Get(), _swapchain->Get(), UINT64_MAX,
                          _synchronizer->GetImageSemaphores()[_currentFrame],
                          VK_NULL_HANDLE, &imageIndex);

    const auto orthoCamera =
            dynamic_cast<OrthographicCamera *>(_cameraManager->currentCamera);
    orthoCamera->setPerspective((float) width, (float) height, xscale, yscale, 0.1f, 256.0f);

    // Begin render pass and command buffer recording
    _commandBuffers[_currentFrame].Record();
    _renderPass->Begin(_commandBuffers[_currentFrame],
                       imageIndex, width,
                       height);

    UboDataDynamic ubodyn{};
    ubodyn.perspective = orthoCamera->matrices.perspective;
    ubodyn.view = orthoCamera->matrices.view;
    memcpy(_UBO->GetMappedMemory(), &ubodyn, sizeof(ubodyn));

    _world->Get()->each<Components::Position>([&](flecs::entity e, Components::Position &position) {
        _sortedEntities.emplace_back(e);
    });

    std::sort(_sortedEntities.begin(), _sortedEntities.end(),
              [](const auto &a, const auto &b) {
                  return CompareZIndex(a, b); // Compare Positions
              });

    for (const auto &e: _sortedEntities) {
        auto position_component = e.get_ref<Components::Position>();
        auto scale_component = e.get_ref<Components::Scale>();
        auto color_component = e.get_ref<Components::Color>();
        auto rotation_component = e.get_ref<Components::Rotation>();
        auto render_component = e.get_ref<Components::Renderable>();

        auto translate = glm::mat4(1.0f);
        auto rotation = glm::mat4(1.0f);
        auto scaling = glm::mat4(1.0f);

        if (position_component.get() != nullptr) {
            translate = glm::translate(glm::mat4(1.0f), position_component->pos);
        }

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

        objectSSBO[render_component->id - 1].bgColor =
                color_component.get() == nullptr
                    ? glm::vec4(1.0, 1.0, 1.0, 1.0)
                    : color_component->color;

        objectSSBO[render_component->id - 1].type = render_component->type;
        objectSSBO[render_component->id - 1].dimension = glm::vec2{scale_component->scale.x, scale_component->scale.y};

        vmaUnmapMemory(_allocator->Get(), _instanceDataBuffer->_allocation);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = {(unsigned int) width, (unsigned int) height};
        vkCmdSetScissor(_commandBuffers[_currentFrame].Get(), 0, 1, &scissor);

        // Set Viewport
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(width);
        viewport.height = static_cast<float>(height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(_commandBuffers[_currentFrame].Get(), 0, 1, &viewport);

        vkCmdBindPipeline(_commandBuffers[_currentFrame].Get(),
                          VK_PIPELINE_BIND_POINT_GRAPHICS,
                          _staticPipeline->GetPipeline());

        const auto ds0 = _staticPipeline->descriptorSet; //;_staticPipeline->descriptorSets[0]->Get()[_currentFrame];

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

        // upload the matrix to the GPU via push constants
        vkCmdPushConstants(_commandBuffers[_currentFrame].Get(),
                           _staticPipeline->GetPipelineLayout(),
                           VK_SHADER_STAGE_ALL, 0, sizeof(PushConstBlock),
                           &constants);

        if (render_component.get() != nullptr &&
            e.get<Components::Renderable>()->indexBuffer == nullptr) {
            const auto renderable = e.get<Components::Renderable>();
            // Bind vertex & index buffers
            const VkBuffer vertexBuffers[] = {renderable->vertexBuffer->GetVulkanBuffer()};
            constexpr VkDeviceSize offsets[] = {0};
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
    }

    PresentForApplication();

    _sortedEntities.clear();

    _currentFrame = (_currentFrame + 1) % MAX_CONCURRENT_FRAMES_IN_FLIGHT;
}
