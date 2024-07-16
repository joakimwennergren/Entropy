#include "vulkan_renderer.hpp"
#include "data/pushcontants.hpp"
#include "data/ubo.hpp"
#include "ecs/components/color.hpp"
#include "ecs/components/hasTexture.hpp"
#include "ecs/components/objmodel.hpp"
#include "ecs/components/position.hpp"
#include "ecs/components/renderable.hpp"
#include "ecs/components/rotation.hpp"
#include "ecs/components/scale.hpp"
#include "ecs/components/sprite.hpp"
#include "glm/gtx/string_cast.hpp"

using namespace Entropy::Graphics::Vulkan::Renderers;

void VulkanRenderer::Render(int width, int height, float xscale, float yscale,
                            bool &needResize, bool app) {

  if (_renderPass._frameBuffers[0] == nullptr) {
    spdlog::error("NO FRAMEBUFFERS YET");
    return;
  }

  if (app) {
    auto currentFence = _synchronizer->GetFences()[_currentFrame];

    vkWaitForFences(_backend.logicalDevice.Get(), 1, &currentFence, VK_TRUE,
                    UINT64_MAX);

    // Reset fences
    vkResetFences(_backend.logicalDevice.Get(), 1, &currentFence);

    auto acquire_result = vkAcquireNextImageKHR(
        _backend.logicalDevice.Get(), _swapChain.Get(), UINT64_MAX,
        _synchronizer->GetImageSemaphores()[_currentFrame], VK_NULL_HANDLE,
        &imageIndex);
  }

  _camera->setPerspective(60.0f, (float)width / (float)height, 0.1f, 256.0f);

  if (needResize) {
    spdlog::info("RESIZING!!");
    stagingBuffer = _bufferFactory.CreateStagingBuffer(
        width * height * 4, nullptr, VK_BUFFER_USAGE_TRANSFER_DST_BIT);

    // Synchronizer
    // delete _synchronizer;
    // _synchronizer = new Synchronizer(_backend.logicalDevice,
    //                                  MAX_CONCURRENT_FRAMES_IN_FLIGHT);
    _swapChain.RecreateSwapChain(width, height);
    _renderPass.RecreateDepthBuffer(width, height);
    _renderPass.RecreateFrameBuffers(width, height, app);

    needResize = false;
  }

  // Submit command buffers
  // if (_queuSync.commandBuffers.size() > 0) {
  //   spdlog::info("submitting cmdbuf");
  //   VkSubmitInfo submitInfo{};
  //   submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  //   submitInfo.commandBufferCount = _queuSync.commandBuffers.size();
  //   submitInfo.pCommandBuffers = _queuSync.commandBuffers.data();
  //   vkQueueSubmit(_backend.logicalDevice.GetGraphicQueue(), 1, &submitInfo,
  //                 nullptr);
  //   _queuSync.commandBuffers.clear();
  // }

  if (_world.gameWorld->count<Entropy::Components::Renderable>() == 0)
    return;

  // Begin renderpass and commandbuffer recording
  _commandBuffers[_currentFrame].Record();
  _renderPass.Begin(_commandBuffers[_currentFrame],
                    app ? imageIndex : VK_SUBPASS_CONTENTS_INLINE, width,
                    height);

  UboDataDynamic ubodyn{};
  ubodyn.perspective = _camera->matrices.perspective;
  ubodyn.view = _camera->matrices.view;
  memcpy(_UBO->GetMappedMemory(), &ubodyn, sizeof(ubodyn));

  _sortQuery.each([this, width, height](flecs::entity e,
                                        Entropy::Components::Renderable r) {
    auto position_component = e.get_ref<Entropy::Components::Position>();
    auto scale_component = e.get_ref<Entropy::Components::Scale>();
    auto color_component = e.get_ref<Entropy::Components::Color>();
    auto rotation_component = e.get_ref<Entropy::Components::Rotation>();
    auto texture_component = e.get_ref<Entropy::Components::HasTexture>();
    auto render_component = e.get_ref<Entropy::Components::Renderable>();

    auto translate = glm::mat4(1.0f);
    auto rotation = glm::mat4(1.0f);
    auto scaling = glm::mat4(1.0f);

    if (position_component.get() != nullptr) {
      translate = glm::translate(glm::mat4(1.0f), position_component->pos);
    }

    if (scale_component.get() != nullptr) {
      scaling = glm::scale(glm::mat4(1.0f), scale_component->scale);
    }

    if (rotation_component.get() != nullptr) {
      rotation =
          glm::rotate(glm::mat4(1.0f), glm::radians(rotation_component->angle),
                      rotation_component->orientation);
    }

    void *objectData;
    vmaMapMemory(_allocator.Get(), _instanceDataBuffer->_allocation,
                 &objectData);

    InstanceData *objectSSBO = (InstanceData *)objectData;
    objectSSBO[r.id - 1].model = (translate * scaling * rotation);
    objectSSBO[r.id - 1].color = color_component.get() == nullptr
                                     ? glm::vec4(1.0, 1.0, 1.0, 1.0)
                                     : color_component->color;

    vmaUnmapMemory(_allocator.Get(), _instanceDataBuffer->_allocation);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = {(unsigned int)width, (unsigned int)height};
    vkCmdSetScissor(_commandBuffers[_currentFrame].Get(), 0, 1, &scissor);

    // Set Viewport
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)width;
    viewport.height = (float)height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(_commandBuffers[_currentFrame].Get(), 0, 1, &viewport);

    vkCmdBindPipeline(_commandBuffers[_currentFrame].Get(),
                      VK_PIPELINE_BIND_POINT_GRAPHICS,
                      _staticPipeline->GetPipeline());

    auto ds0 = _staticPipeline->descriptorSets[0].Get()[_currentFrame];

    vkCmdBindDescriptorSets(
        _commandBuffers[_currentFrame].Get(), VK_PIPELINE_BIND_POINT_GRAPHICS,
        _staticPipeline->GetPipelineLayout(), 0, 1, &ds0, 0, nullptr);

    if (e.has<Entropy::Components::HasTexture>()) {

      auto texture = e.get<Entropy::Components::HasTexture>();

      vkCmdBindDescriptorSets(_commandBuffers[_currentFrame].Get(),
                              VK_PIPELINE_BIND_POINT_GRAPHICS,
                              _staticPipeline->GetPipelineLayout(), 1, 1,
                              &texture->texture->_descriptorSet, 0, nullptr);
    }

    PushConstBlock constants;
    constants.instanceIndex = r.id - 1;

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
        e.get<Entropy::Components::Renderable>()->indexBuffer != nullptr) {
      auto renderable = e.get<Entropy::Components::Renderable>();
      // Bind vertex & index buffers
      VkBuffer vertexBuffers[] = {renderable->vertexBuffer->GetVulkanBuffer()};
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

  if (app) {
    PresentForApplication();
  } else {
    PresentForEditor(width, height);
  }

  _currentFrame = (_currentFrame + 1) % MAX_CONCURRENT_FRAMES_IN_FLIGHT - 1;
}
