#pragma once

#include "factories/vulkan/bufferfactory.hpp"
#include "obj/model.hpp"
#include <graphics/vulkan/vulkan_backend.hpp>
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <flecs/flecs.h>

#include <graphics/data/pushcontants.hpp>
#include <graphics/data/ubo.hpp>
#include <graphics/data/vertex.hpp>
#include <graphics/vulkan/buffers/indexbuffer.hpp>
#include <graphics/vulkan/buffers/uniformbuffer.hpp>
#include <graphics/vulkan/commandbuffers/commandbuffer.hpp>
#include <graphics/vulkan/pipelines/static_pipeline.hpp>
#include <graphics/vulkan/renderpasses/renderpass.hpp>
#include <graphics/vulkan/synchronization/synchronizer.hpp>
// #include <graphics/cubemaps/cubemap.hpp>
#include <gltf/model.hpp>
#include <graphics/cameras/perspective_camera.hpp>
#include <graphics/primitives/3d/plane.hpp>
#include <graphics/vulkan/descriptorsets/descriptorset.hpp>
#include <graphics/vulkan/swapchains/swapchain.hpp>

#include <graphics/cameras/perspective_camera.hpp>

#include <graphics/primitives/2d/quad.hpp>
#include <graphics/primitives/2d/sprite.hpp>
#include <graphics/vulkan/buffers/uniformbuffer.hpp>

#include <graphics/cameras/flying_camera.hpp>

#include <input/keyboard/keyboard.hpp>

#include <graphics/data/ubo.hpp>
#include <graphics/vulkan/buffers/stagedbuffer.hpp>
#include <graphics/vulkan/synchronization/queuesync.hpp>

#include <tracy/Tracy.hpp>

#include <ecs/components/color.hpp>
#include <ecs/components/gizmo.hpp>
#include <ecs/components/line.hpp>
#include <ecs/components/model.hpp>
#include <ecs/components/objmodel.hpp>
#include <ecs/components/position.hpp>
#include <ecs/components/renderable.hpp>
#include <ecs/components/rotation.hpp>
#include <ecs/components/scale.hpp>
#include <ecs/components/tags/scripted.hpp>

#include <timing/timer.hpp>

#ifdef BUILD_FOR_ANDROID
#include <android/asset_manager.h>
#endif
#include <factories/vulkan/pipelinefactory.hpp>

using namespace Entropy::Graphics::Vulkan::Buffers;
using namespace Entropy::Graphics::Vulkan::Textures;
using namespace Entropy::Graphics::Vulkan::Pipelines;
using namespace Entropy::Graphics::Vulkan::RenderPasses;
using namespace Entropy::Graphics::Vulkan::CommandBuffers;
using namespace Entropy::Graphics::Vulkan::Synchronization;
using namespace Entropy::Graphics::Vulkan::Descriptorsets;
using namespace Entropy::Graphics::Vulkan::Swapchains;
using namespace Entropy::Graphics::Primitives;
using namespace Entropy::Graphics::Vulkan::Synchronization;
using namespace Entropy::GLTF;
// using namespace Entropy::Graphics::CubeMaps;
using namespace Entropy::Input;

using namespace Entropy::Factories::Vulkan;

namespace Entropy {
namespace Graphics {
namespace Renderers {

struct VulkanRenderer {

  VulkanRenderer(Vulkan::VulkanBackend vbe, QueueSync queueSync,
                 RenderPass renderPass, PipelineFactory pipelineFactory,
                 BufferFactory bf, CommandPool cp, Swapchain sc)

      : _backend{vbe}, _queuSync{queueSync}, _renderPass{renderPass},
        _pipelineFactory{pipelineFactory}, _bufferFactory{bf}, _commandPool{cp},
        _swapChain{sc} {

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

    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(_backend.physicalDevice.Get(), &properties);
    size_t minUboAlignment = properties.limits.minUniformBufferOffsetAlignment;
    dynamicAlignment = sizeof(UboDataDynamic);

    if (minUboAlignment > 0) {
      dynamicAlignment =
          (dynamicAlignment + minUboAlignment - 1) & ~(minUboAlignment - 1);
    }

    for (size_t i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++) {
      std::array<VkWriteDescriptorSet, 1> descriptorWrites{};

      VkDescriptorBufferInfo bufferInfo{};
      bufferInfo.buffer = _dynamicUBO->GetVulkanBuffer();
      bufferInfo.offset = 0;
      bufferInfo.range = sizeof(UboDataDynamic);

      descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      descriptorWrites[0].dstSet = _staticPipeline->descriptorSets[0].Get()[i];
      descriptorWrites[0].dstBinding = 1;
      descriptorWrites[0].dstArrayElement = 0;
      descriptorWrites[0].descriptorType =
          VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
      descriptorWrites[0].descriptorCount = 1;
      descriptorWrites[0].pBufferInfo = &bufferInfo;

      vkUpdateDescriptorSets(_backend.logicalDevice.Get(),
                             static_cast<uint32_t>(descriptorWrites.size()),
                             descriptorWrites.data(), 0, nullptr);
    }

    // StagingBuffer
    _stagingBuffer = _bufferFactory.CreateStagingBuffer(800 * 800 * 4, nullptr);

    // Camera
    _camera = new PerspectiveCamera();
    _camera->type = PerspectiveCamera::CameraType::firstperson;
    _camera->setPosition(glm::vec3(0.0f, 0.0f, -500.0f));
    _camera->setRotation(glm::vec3(0.0f));

    // @todo temp!
    _model = std::make_shared<Entropy::OBJ::ObjModel>(_backend, _bufferFactory);
    _model->loadFromFile("/Users/joakim/Desktop/models/12140_Skull_v3_L2.obj",
                         "/Users/joakim/Desktop/models/Skull.png");
  }

  ~VulkanRenderer() {
    delete _dynamicUBO;
    delete _staticPipeline;
    delete _camera;
    delete _synchronizer;
  }

  void Render(int width, int height, float xscale, float yscale);
  StagedBuffer *_stagingBuffer;

protected:
  PerspectiveCamera *_camera;

  // @todo temp!
  float z = 0.0;
  std::shared_ptr<Entropy::OBJ::ObjModel> _model;

private:
  uint32_t _currentFrame = 0;
  size_t dynamicAlignment = 0;

  std::vector<CommandBuffer> _commandBuffers;
  UniformBuffer *_dynamicUBO;
  StaticPipeline *_staticPipeline;
  Synchronizer *_synchronizer;

  // Dependencies
  Vulkan::VulkanBackend _backend;
  QueueSync _queuSync;
  RenderPass _renderPass;
  PipelineFactory _pipelineFactory;
  BufferFactory _bufferFactory;
  CommandPool _commandPool;
  Swapchain _swapChain;
};
} // namespace Renderers
} // namespace Graphics
} // namespace Entropy
