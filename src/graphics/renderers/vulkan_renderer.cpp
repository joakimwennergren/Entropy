#include "vulkan_renderer.hpp"
#include "data/ubo.hpp"
#include "ecs/components/color.hpp"
#include "ecs/components/hasTexture.hpp"
#include "ecs/components/objmodel.hpp"
#include "ecs/components/position.hpp"
#include "ecs/components/rotation.hpp"
#include "ecs/components/scale.hpp"
#include "ecs/components/sprite.hpp"
#include "vulkan/vulkan_core.h"

using namespace Entropy::Graphics::Vulkan::Renderers;

void VulkanRenderer::Render(int width, int height, float xscale, float yscale,
                            bool &needResize) {

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
    _renderPass.RecreateFrameBuffers(width, height);

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

  _world.gameWorld->each<Entropy::Components::Renderable>(
      [this](flecs::entity e, Entropy::Components::Renderable r) {
        if (e.has<Entropy::Components::HasTexture>()) {

          auto texture = e.get<Entropy::Components::HasTexture>();

          std::array<VkWriteDescriptorSet, 1> descriptorWrites{};

          VkDescriptorImageInfo imageInfo{};
          imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
          imageInfo.imageView = texture->texture->_imageView;
          imageInfo.sampler = _sampler;

          descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
          descriptorWrites[0].dstSet =
              _staticPipeline->descriptorSets[0].Get()[0];
          descriptorWrites[0].dstBinding = 2;
          descriptorWrites[0].dstArrayElement =
              e.get_ref<Entropy::Components::HasTexture>()->texture->textureId -
              1;
          descriptorWrites[0].descriptorType =
              VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
          descriptorWrites[0].descriptorCount = 1;
          descriptorWrites[0].pImageInfo = &imageInfo;

          vkUpdateDescriptorSets(_backend.logicalDevice.Get(),
                                 static_cast<uint32_t>(descriptorWrites.size()),
                                 descriptorWrites.data(), 0, nullptr);
        }
      });

  // Begin renderpass and commandbuffer recording
  _commandBuffers[_currentFrame].Record();
  _renderPass.Begin(_commandBuffers[_currentFrame], VK_SUBPASS_CONTENTS_INLINE,
                    width, height);

  UboDataDynamic ubodyn{};

  ubodyn.perspective = _camera->matrices.perspective;
  ubodyn.view = _camera->matrices.view;

  memcpy(_dynamicUBO->GetMappedMemory(), &ubodyn, sizeof(ubodyn));

  _world.gameWorld->each<Entropy::Components::Renderable>(
      [this, width, height](flecs::entity e,
                            Entropy::Components::Renderable r) {
        auto position_component = e.get_ref<Entropy::Components::Position>();
        auto scale_component = e.get_ref<Entropy::Components::Scale>();
        auto color_component = e.get_ref<Entropy::Components::Color>();
        auto rotation_component = e.get_ref<Entropy::Components::Rotation>();
        auto texture_component = e.get_ref<Entropy::Components::HasTexture>();

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
          rotation = glm::rotate(glm::mat4(1.0f),
                                 glm::radians(rotation_component->angle),
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

        if (texture_component.get() != nullptr) {
          objectSSBO[r.id - 1].textureId =
              texture_component->texture->textureId - 1;
        }

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

        vkCmdBindDescriptorSets(_commandBuffers[_currentFrame].Get(),
                                VK_PIPELINE_BIND_POINT_GRAPHICS,
                                _staticPipeline->GetPipelineLayout(), 0, 1,
                                &ds0, 0, nullptr);

        if (e.has<Entropy::Components::OBJModel>()) {
          auto model = e.get<Entropy::Components::OBJModel>();
          // Bind vertex & index buffers
          VkBuffer vertexBuffers[] = {
              model->model->vertexBuffer->GetVulkanBuffer()};
          VkDeviceSize offsets[] = {0};
          vkCmdBindVertexBuffers(_commandBuffers[_currentFrame].Get(), 0, 1,
                                 vertexBuffers, offsets);

          vkCmdDraw(_commandBuffers[_currentFrame].Get(),
                    model->model->vertices.size(), 1, 0, r.id - 1);
        }

        if (e.has<Entropy::Components::SpriteComponent>()) {
          auto sprite = e.get<Entropy::Components::SpriteComponent>();
          // Bind vertex & index buffers
          VkBuffer vertexBuffers[] = {
              sprite->sprite->vertexBuffer->GetVulkanBuffer()};
          VkDeviceSize offsets[] = {0};
          vkCmdBindVertexBuffers(_commandBuffers[_currentFrame].Get(), 0, 1,
                                 vertexBuffers, offsets);

          vkCmdBindIndexBuffer(_commandBuffers[_currentFrame].Get(),
                               sprite->sprite->indexBuffer->GetVulkanBuffer(),
                               0, VK_INDEX_TYPE_UINT16);
          // Draw current renderable
          vkCmdDrawIndexed(_commandBuffers[_currentFrame].Get(),
                           sprite->sprite->indices.size(), 1, 0, 0, r.id - 1);
        }
      });

  // End renderpass and commandbuffer recording
  _renderPass.End(_commandBuffers[_currentFrame]);

  VkImageLayout oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  VkImageLayout newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;

  /* Synchronize image access. */
  VkImageMemoryBarrier barrier{
      .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
      .srcAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
      .dstAccessMask = VK_ACCESS_2_TRANSFER_READ_BIT,
      .oldLayout = oldLayout,
      .newLayout = newLayout,
      .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      .image = _renderPass._swapChainTextures[0]->_textureImage,
      .subresourceRange =
          {
              .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
              .baseMipLevel = 0,
              .levelCount = 1,
              .baseArrayLayer = 0,
              .layerCount = 1,
          },
  };

  VkPipelineStageFlags sourceStage;
  VkPipelineStageFlags destinationStage;

  if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
      newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) {
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

    sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
  } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
             newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
  } else {
    throw std::invalid_argument("unsupported layout transition!");
  }

  vkCmdPipelineBarrier(_commandBuffers[_currentFrame].Get(), sourceStage,
                       destinationStage, 0, 0, nullptr, 0, nullptr, 1,
                       &barrier);

  /* Copy framebuffer content to staging buffer. */
  VkBufferImageCopy region{
      .bufferOffset = 0,
      .bufferRowLength = 0,
      .bufferImageHeight = 0,

      .imageSubresource =
          {
              .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
              .mipLevel = 0,
              .baseArrayLayer = 0,
              .layerCount = 1,
          },
      .imageOffset = {0, 0, 0},
      .imageExtent = {static_cast<uint32_t>(width),
                      static_cast<uint32_t>(height), 1},
  };
  vkCmdCopyImageToBuffer(_commandBuffers[_currentFrame].Get(),
                         _renderPass._swapChainTextures[0]->_textureImage,
                         VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                         stagingBuffer->GetVulkanBuffer(), 1, &region);

  _commandBuffers[_currentFrame].EndRecording();

  auto cmdBuf = _commandBuffers[_currentFrame].Get();
  VkSubmitInfo submitInfo{
      .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
      .waitSemaphoreCount = 0,
      .commandBufferCount = 1,
      .pCommandBuffers = &cmdBuf,
      .signalSemaphoreCount = 0,
  };

  if (vkQueueSubmit(_backend.logicalDevice.GetGraphicQueue(), 1, &submitInfo,
                    nullptr) != VK_SUCCESS) {
    exit(EXIT_FAILURE);
  }

  vkDeviceWaitIdle(_backend.logicalDevice.Get());
}