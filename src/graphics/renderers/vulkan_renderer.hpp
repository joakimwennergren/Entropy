#pragma once

#include "cameras/camera_manager.hpp"
#include "cameras/ortho_camera.hpp"
#include "config.hpp"
#include "ecs/components/position.hpp"
#include "graphics/vulkan/memory/allocator.hpp"
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cameras/perspective_camera.hpp>
#include <data/ubo.hpp>
#include <ecs/world.hpp>
#include <factories/vulkan/bufferfactory.hpp>
#include <factories/vulkan/pipelinefactory.hpp>
#include <graphics/vulkan/buffers/stagedbuffer.hpp>
#include <graphics/vulkan/buffers/storagebuffer.hpp>
#include <graphics/vulkan/buffers/uniformbuffer.hpp>
#include <graphics/vulkan/commandbuffers/commandbuffer.hpp>
#include <graphics/vulkan/pipelines/static_pipeline.hpp>
#include <graphics/vulkan/renderpasses/renderpass.hpp>
#include <graphics/vulkan/swapchains/swapchain.hpp>
#include <graphics/vulkan/synchronization/queuesync.hpp>
#include <graphics/vulkan/synchronization/synchronizer.hpp>
#include <graphics/vulkan/vulkan_backend.hpp>
#include <obj/model.hpp>

#include <ecs/components/renderable.hpp>
#include <ecs/components/sprite.hpp>

#include <flecs/flecs.h>
#include <timing/timer.hpp>
#include <tracy/Tracy.hpp>

#ifdef BUILD_FOR_ANDROID
#include <android/asset_manager.h>
#endif

using namespace Entropy::Graphics::Vulkan::Buffers;
using namespace Entropy::Graphics::Vulkan::Textures;
using namespace Entropy::Graphics::Vulkan::Pipelines;
using namespace Entropy::Graphics::Vulkan::RenderPasses;
using namespace Entropy::Graphics::Vulkan::CommandBuffers;
using namespace Entropy::Graphics::Vulkan::Synchronization;
using namespace Entropy::Graphics::Vulkan::Descriptorsets;
using namespace Entropy::Graphics::Vulkan::Swapchains;
using namespace Entropy::Graphics::Vulkan::Synchronization;
using namespace Entropy::Factories::Vulkan;
using namespace Entropy::Data;
using namespace Entropy::ECS;

using namespace Entropy::Cameras;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Renderers {

/**
 * @brief Vulkan Renderer
 * @author Joakim Wennergren
 * @since Thu Jul 04 2024
 */
struct VulkanRenderer {

  /**
   * @brief Constructor for vulkan renderer
   * @param vbe VulkanBackend
   * @param queueSync QueueSync
   * @param renderPass RenderPass
   * @param pipelineFactory PipelineFactory
   * @param bf BufferFactory
   * @param cp CommandPool
   * @param sc Swapchain
   */
  VulkanRenderer(Vulkan::VulkanBackend vbe, QueueSync queueSync,
                 RenderPass renderPass, PipelineFactory pipelineFactory,
                 BufferFactory bf, CommandPool cp, Swapchain sc, World world,
                 Allocator allocator, CameraManager cm)

      : _backend{vbe}, _queuSync{queueSync}, _renderPass{renderPass},
        _pipelineFactory{pipelineFactory}, _bufferFactory{bf}, _commandPool{cp},
        _swapChain{sc}, _world{world}, _allocator{allocator}, _cameraManager{
                                                                  cm} {

    // Static Pipeline creation
    _staticPipeline = _pipelineFactory.CreateStaticPipeline();

    // Synchronizer
    _synchronizer =
        new Synchronizer(vbe.logicalDevice, MAX_CONCURRENT_FRAMES_IN_FLIGHT);

    // Command buffers
    for (uint32_t i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++) {
      _commandBuffers.push_back(CommandBuffer(_backend, _queuSync, _commandPool,
                                              VK_COMMAND_BUFFER_LEVEL_PRIMARY));
    }

    // UBO
    _UBO = _bufferFactory.CreateUniformBuffer(sizeof(UboDataDynamic));

    _instanceDataBuffer = _bufferFactory.CreateStorageBuffer(
        MAX_INSTANCE_COUNT * sizeof(InstanceData), nullptr);

    // Update descriptorsets
    for (size_t i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++) {
      std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

      VkDescriptorBufferInfo bufferInfo{};
      bufferInfo.buffer = _UBO->GetVulkanBuffer();
      bufferInfo.offset = 0;
      bufferInfo.range = sizeof(UboDataDynamic);

      VkDescriptorBufferInfo objectBufferInfo;
      objectBufferInfo.buffer = _instanceDataBuffer->GetVulkanBuffer();
      objectBufferInfo.offset = 0;
      objectBufferInfo.range = sizeof(InstanceData) * MAX_INSTANCE_COUNT;

      descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      descriptorWrites[0].dstSet = _staticPipeline->descriptorSets[0].Get()[i];
      descriptorWrites[0].dstBinding = 1;
      descriptorWrites[0].dstArrayElement = 0;
      descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      descriptorWrites[0].descriptorCount = 1;
      descriptorWrites[0].pBufferInfo = &bufferInfo;

      descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      descriptorWrites[1].dstSet = _staticPipeline->descriptorSets[0].Get()[i];
      descriptorWrites[1].dstBinding = 0;
      descriptorWrites[1].dstArrayElement = 0;
      descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
      descriptorWrites[1].descriptorCount = 1;
      descriptorWrites[1].pBufferInfo = &objectBufferInfo;

      vkUpdateDescriptorSets(_backend.logicalDevice.Get(),
                             static_cast<uint32_t>(descriptorWrites.size()),
                             descriptorWrites.data(), 0, nullptr);
    }

    // StagingBuffer
    stagingBuffer = _bufferFactory.CreateStagingBuffer(
        800 * 800 * 4, nullptr, VK_BUFFER_USAGE_TRANSFER_DST_BIT);

    // Camera
    _camera = std::make_shared<PerspectiveCamera>();
    _camera->type = PerspectiveCamera::CameraType::firstperson;
    _camera->setPosition(glm::vec3(0.0f, 0.0f, -200.0));
    _camera->setRotation(glm::vec3(0.0f));

    // Create a query for the Position component with a custom sorting function
    _sortQuery = world.gameWorld->query_builder<Components::Renderable>()
                     .order_by<Components::Renderable>(compare_zIndex)
                     .build();
  }

  /**
   * @brief Destructor for Vulkan Renderer
   */
  ~VulkanRenderer() {
    // delete _dynamicUBO;
    // delete _staticPipeline;
    // delete _camera;
    // delete _synchronizer;
  }

  /**
   * @brief Render method
   * @param width
   * @param height
   * @param xscale
   * @param yscale
   * @param needResize
   * @return (void)
   */
  void Render(int width, int height, float xscale, float yscale,
              bool &needResize, bool app);

  // Comparison function for sorting based on z index
  static int compare_zIndex(flecs::entity_t e1,
                            const Components::Renderable *p1,
                            flecs::entity_t e2,
                            const Components::Renderable *p2) {
    return (p1->zIndex > p2->zIndex) - (p1->zIndex < p2->zIndex);
  }

  void PresentForEditor(int width, int height) {

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

  void PresentForApplication() {
    // End renderpass and commandbuffer recording
    //     // End renderpass and commandbuffer recording
    _renderPass.End(_commandBuffers[_currentFrame]);
    _commandBuffers[_currentFrame].EndRecording();

    auto cmdBuffer = _commandBuffers[_currentFrame].Get();

    // Submit info
    VkSubmitInfo submitInfo{};
    VkSemaphore signalSemaphores[] = {
        _synchronizer->GetRenderFinishedSemaphores()[_currentFrame]};
    VkSemaphore waitSemaphores[] = {
        _synchronizer->GetImageSemaphores()[_currentFrame]};
    VkPipelineStageFlags waitStages[] = {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

    std::vector<VkCommandBuffer> submittables = {cmdBuffer};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = submittables.size();
    submitInfo.pCommandBuffers = submittables.data();
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    // Submit queue
    if (vkQueueSubmit(_backend.logicalDevice.GetGraphicQueue(), 1, &submitInfo,
                      _synchronizer->GetFences()[_currentFrame]) !=
        VK_SUCCESS) {
      exit(EXIT_FAILURE);
    }

    // PresentInfo
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    // SwapChains
    VkSwapchainKHR swapChains[] = {_swapChain.Get()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    // Present
    vkQueuePresentKHR(_backend.logicalDevice.GetGraphicQueue(), &presentInfo);
  }

  std::shared_ptr<StagedBuffer> stagingBuffer;
  Vulkan::VulkanBackend _backend;
  Swapchain _swapChain;
  RenderPass _renderPass;
  CameraManager _cameraManager;

private:
  flecs::query<Components::Renderable> _sortQuery;

  // @todo
  std::shared_ptr<PerspectiveCamera> _camera;
  uint32_t _currentFrame = 0;
  uint32_t imageIndex;

  // Command Buffers
  std::vector<CommandBuffer> _commandBuffers;
  // Dynamic UBO
  std::shared_ptr<UniformBuffer> _UBO;

  std::shared_ptr<StorageBuffer> _instanceDataBuffer;
  // Pipelines
  StaticPipeline *_staticPipeline;
  // Synchronizer
  Synchronizer *_synchronizer;

  // Dependencies

  QueueSync _queuSync;

  PipelineFactory _pipelineFactory;
  BufferFactory _bufferFactory;
  CommandPool _commandPool;

  World _world;
  Allocator _allocator;
  VkSampler _sampler;
};
} // namespace Renderers
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy
