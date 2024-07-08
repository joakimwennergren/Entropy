#pragma once

#include "config.hpp"
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
                 Allocator allocator)

      : _backend{vbe}, _queuSync{queueSync}, _renderPass{renderPass},
        _pipelineFactory{pipelineFactory}, _bufferFactory{bf}, _commandPool{cp},
        _swapChain{sc}, _world{world}, _allocator{allocator} {

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

    // Dynamic UBO
    _dynamicUBO =
        _bufferFactory.CreateUniformBuffer(sizeof(UboDataDynamic) * 10000);

    _instanceDataBuffer = _bufferFactory.CreateStorageBuffer(
        MAX_INSTANCE_COUNT * sizeof(InstanceData), nullptr);

    // Update descriptorsets
    for (size_t i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++) {
      std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

      VkDescriptorBufferInfo bufferInfo{};
      bufferInfo.buffer = _dynamicUBO->GetVulkanBuffer();
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
    _camera->setPosition(glm::vec3(0.0f, 0.0f, -100.0));
    _camera->setRotation(glm::vec3(0.0f));

    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(_backend.physicalDevice.Get(), &properties);

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

    if (vkCreateSampler(_backend.logicalDevice.Get(), &samplerInfo, nullptr,
                        &_sampler) != VK_SUCCESS) {
      throw std::runtime_error("failed to create texture sampler!");
    }
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
              bool &needResize);

  std::shared_ptr<StagedBuffer> stagingBuffer;

private:
  // @todo
  std::shared_ptr<PerspectiveCamera> _camera;
  uint32_t _currentFrame = 0;

  // Command Buffers
  std::vector<CommandBuffer> _commandBuffers;
  // Dynamic UBO
  UniformBuffer *_dynamicUBO;

  std::shared_ptr<StorageBuffer> _instanceDataBuffer;
  // Pipelines
  StaticPipeline *_staticPipeline;
  // Synchronizer
  Synchronizer *_synchronizer;

  // Dependencies
  Vulkan::VulkanBackend _backend;
  QueueSync _queuSync;
  RenderPass _renderPass;
  PipelineFactory _pipelineFactory;
  BufferFactory _bufferFactory;
  CommandPool _commandPool;
  Swapchain _swapChain;
  World _world;
  Allocator _allocator;
  VkSampler _sampler;
};
} // namespace Renderers
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy
