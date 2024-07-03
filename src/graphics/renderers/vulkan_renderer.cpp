#include "vulkan_renderer.hpp"

using namespace Entropy::Graphics::Renderers;

void VulkanRenderer::Render(int width, int height, float xscale, float yscale,
                            bool needResize) {

  _camera->setPerspective(60.0f, (float)width / (float)height, 0.1f, 100000.0f);

  if (needResize) {
    delete _stagingBuffer;
    _stagingBuffer =
        _bufferFactory.CreateStagingBuffer(width * height * 4, nullptr);

    // Synchronizer
    delete _synchronizer;
    _synchronizer = new Synchronizer(_backend.logicalDevice,
                                     MAX_CONCURRENT_FRAMES_IN_FLIGHT);
    _swapChain.RecreateSwapChain(width, height);
    _renderPass.RecreateDepthBuffer(width, height);
    _renderPass.RecreateFrameBuffers(width, height);

    needResize = false;
  }

  // Submit command buffers
  if (_queuSync.commandBuffers.size() > 0) {
    spdlog::info("submitting cmdbuf");
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = _queuSync.commandBuffers.size();
    submitInfo.pCommandBuffers = _queuSync.commandBuffers.data();
    vkQueueSubmit(_backend.logicalDevice.GetGraphicQueue(), 1, &submitInfo,
                  nullptr);
    _queuSync.commandBuffers.clear();
  }

  // Begin renderpass and commandbuffer recording
  _commandBuffers[_currentFrame].Record();
  _renderPass.Begin(_commandBuffers[_currentFrame], VK_SUBPASS_CONTENTS_INLINE,
                    width, height);

  UboDataDynamic ubodyn{};

  auto model =
      glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 160.0, 0.0)) *
      glm::scale(glm::mat4(1.0f), glm::vec3{14.0, 14.0, 14.0}) *
      glm::rotate(glm::mat4(1.0f), glm::radians(z), glm::vec3{0.0, 1.0, 0.0}) *

      glm::rotate(glm::mat4(1.0f), glm::radians(90.0f),
                  glm::vec3{1.0, 0.0, 0.0});

  ubodyn.proj = _camera->matrices.perspective * _camera->matrices.view * model;

  uint32_t offset = dynamicAlignment * 0;
  memcpy((char *)_dynamicUBO->GetMappedMemory() + offset, &ubodyn,
         sizeof(UboDataDynamic));

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
      _staticPipeline->GetPipelineLayout(), 0, 1, &ds0, 1, &offset);

  // vkCmdBindDescriptorSets(_commandBuffers[_currentFrame].GetCommandBuffer(),
  //                         VK_PIPELINE_BIND_POINT_GRAPHICS,
  //                         _staticPipeline->GetPipelineLayout(), 1, 1,
  //                         &_model->ds, 0, nullptr);

  // // Bind vertex & index buffers
  // // Bind descriptor sets
  VkBuffer vertexBuffers[] = {_model->vertexBuffer->GetVulkanBuffer()};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(_commandBuffers[_currentFrame].Get(), 0, 1,
                         vertexBuffers, offsets);

  vkCmdDraw(_commandBuffers[_currentFrame].Get(), _model->vertices.size(), 1, 0,
            0);

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
      .image = _renderPass._swapChainTextures[0]._swapChainImage,
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
                         _renderPass._swapChainTextures[0]._swapChainImage,
                         VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                         _stagingBuffer->GetVulkanBuffer(), 1, &region);

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

  z += 0.5;
}