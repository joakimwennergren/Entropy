#include "renderer.hpp"

using namespace Entropy::Graphics::Renderers;

size_t Renderer::pad_uniform_buffer_size(size_t originalSize) {
  // Calculate required alignment based on minimum device offset alignment
  size_t minUboAlignment = properties.limits.minUniformBufferOffsetAlignment;
  size_t alignedSize = originalSize;
  if (minUboAlignment > 0) {
    alignedSize = (alignedSize + minUboAlignment - 1) & ~(minUboAlignment - 1);
  }
  return alignedSize;
}

void Renderer::Setup(std::shared_ptr<ServiceLocator> serviceLocator,
                     float xscale, float yscale) {
  // Store service locator
  _serviceLocator = serviceLocator;

  // Get required depenencies
  _logicalDevice = serviceLocator->GetService<LogicalDevice>();
  _swapChain = serviceLocator->GetService<Swapchain>();
  _keyboard = serviceLocator->GetService<Keyboard>();
  _cam = serviceLocator->GetService<Cam>();
  auto physicalDevice = serviceLocator->GetService<PhysicalDevice>();

  // Create synchronizer
  //_synchronizer = std::make_shared<Synchronizer>(
  //    MAX_CONCURRENT_FRAMES_IN_FLIGHT, serviceLocator);

  for (uint32_t i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++) {
    _commandBuffers.push_back(std::make_shared<CommandBuffer>(
        serviceLocator, VK_COMMAND_BUFFER_LEVEL_PRIMARY));
  }

  // Create buffers @todo temp!!!
  for (size_t i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++) {
    // _uniformBuffers.push_back(new
    // UniformBuffer(sizeof(UniformBufferObject))); dynUbos.push_back(new
    // UniformBuffer(sizeof(UboDataDynamic) * 10000));
  }

  for (unsigned int i = 0; i < _uniformBuffers.size(); i++) {
    auto rawBuffer = _uniformBuffers[i]->GetVulkanBuffer();
    rawUniformBuffers.push_back(rawBuffer);
  }

  vkGetPhysicalDeviceProperties(physicalDevice->Get(), &properties);
  size_t minUboAlignment = properties.limits.minUniformBufferOffsetAlignment;
  dynamicAlignment = sizeof(UboDataDynamic);

  if (minUboAlignment > 0) {
    dynamicAlignment =
        (dynamicAlignment + minUboAlignment - 1) & ~(minUboAlignment - 1);
  }

  for (size_t i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++) {
    std::array<VkWriteDescriptorSet, 9> descriptorWrites{};

    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = rawUniformBuffers[i];
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(UniformBufferObject);

    VkDescriptorBufferInfo bufferInfo2{};
    bufferInfo2.buffer = dynUbos[i]->GetVulkanBuffer();
    bufferInfo2.offset = 0;
    bufferInfo2.range = sizeof(UboDataDynamic);

    descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[0].dstSet =
        _pipelines["StaticPipeline"]->descriptorSets[0]->Get()[i];
    descriptorWrites[0].dstBinding = 1;
    descriptorWrites[0].dstArrayElement = 0;
    descriptorWrites[0].descriptorType =
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    descriptorWrites[0].descriptorCount = 1;
    descriptorWrites[0].pBufferInfo = &bufferInfo2;

    descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[1].dstSet =
        _pipelines["SkinnedPipeline"]->descriptorSets[0]->Get()[i];
    descriptorWrites[1].dstBinding = 0;
    descriptorWrites[1].dstArrayElement = 0;
    descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrites[1].descriptorCount = 1;
    descriptorWrites[1].pBufferInfo = &bufferInfo;

    descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[2].dstSet =
        _pipelines["Pipeline2D"]->descriptorSets[0]->Get()[i];
    descriptorWrites[2].dstBinding = 0;
    descriptorWrites[2].dstArrayElement = 0;
    descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrites[2].descriptorCount = 1;
    descriptorWrites[2].pBufferInfo = &bufferInfo;

    descriptorWrites[3].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[3].dstSet =
        _pipelines["GizmoPipeline"]->descriptorSets[0]->Get()[i];
    descriptorWrites[3].dstBinding = 0;
    descriptorWrites[3].dstArrayElement = 0;
    descriptorWrites[3].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrites[3].descriptorCount = 1;
    descriptorWrites[3].pBufferInfo = &bufferInfo;

    descriptorWrites[4].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[4].dstSet =
        _pipelines["LinePipeline"]->descriptorSets[0]->Get()[i];
    descriptorWrites[4].dstBinding = 0;
    descriptorWrites[4].dstArrayElement = 0;
    descriptorWrites[4].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrites[4].descriptorCount = 1;
    descriptorWrites[4].pBufferInfo = &bufferInfo;

    descriptorWrites[5].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[5].dstSet =
        _pipelines["SkinnedPipeline"]->descriptorSets[0]->Get()[i];
    descriptorWrites[5].dstBinding = 1;
    descriptorWrites[5].dstArrayElement = 0;
    descriptorWrites[5].descriptorType =
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    descriptorWrites[5].descriptorCount = 1;
    descriptorWrites[5].pBufferInfo = &bufferInfo2;

    descriptorWrites[6].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[6].dstSet =
        _pipelines["Pipeline2D"]->descriptorSets[0]->Get()[i];
    descriptorWrites[6].dstBinding = 1;
    descriptorWrites[6].dstArrayElement = 0;
    descriptorWrites[6].descriptorType =
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    descriptorWrites[6].descriptorCount = 1;
    descriptorWrites[6].pBufferInfo = &bufferInfo2;

    descriptorWrites[7].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[7].dstSet =
        _pipelines["GizmoPipeline"]->descriptorSets[0]->Get()[i];
    descriptorWrites[7].dstBinding = 1;
    descriptorWrites[7].dstArrayElement = 0;
    descriptorWrites[7].descriptorType =
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    descriptorWrites[7].descriptorCount = 1;
    descriptorWrites[7].pBufferInfo = &bufferInfo2;

    descriptorWrites[8].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[8].dstSet =
        _pipelines["LinePipeline"]->descriptorSets[0]->Get()[i];
    descriptorWrites[8].dstBinding = 1;
    descriptorWrites[8].dstArrayElement = 0;
    descriptorWrites[8].descriptorType =
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    descriptorWrites[8].descriptorCount = 1;
    descriptorWrites[8].pBufferInfo = &bufferInfo2;

    vkUpdateDescriptorSets(_logicalDevice->Get(),
                           static_cast<uint32_t>(descriptorWrites.size()),
                           descriptorWrites.data(), 0, nullptr);
  }

  /*
    _camera = std::make_shared<Camera>();
    _camera->type = Camera::CameraType::firstperson;
    _camera->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    _camera->setRotation(glm::vec3(0.0f));
    */
}

#ifdef BUILD_FOR_ANDROID

std::vector<char> Renderer::loadShader(std::string filename,
                                       AAssetManager *assetManager) {
  AAsset *file =
      AAssetManager_open(assetManager, filename.c_str(), AASSET_MODE_BUFFER);
  size_t fileLength = AAsset_getLength(file);
  char *fileContent = new char[fileLength];
  AAsset_read(file, fileContent, fileLength);
  AAsset_close(file);

  std::vector<char> code;

  for (uint32_t i = 0; i < fileLength; i++) {
    code.push_back(fileContent[i]);
  }
  return code;
}

Renderer::Renderer(std::shared_ptr<ServiceLocator> serviceLocator,
                   AAssetManager *assetManager) {
  /*
      auto skinnedVert = loadShader("skinned_vert.spv", assetManager);
      auto skinnedFrag = loadShader("skinned_frag.spv", assetManager);

      auto cubemapVert = loadShader("cubemap_vert.spv", assetManager);
      auto cubemapFrag = loadShader("cubemap_frag.spv", assetManager);

      auto twodVert = loadShader("2d_vert.spv", assetManager);
      auto twodFrag = loadShader("2d_frag.spv", assetManager);

      // Create renderpass
      _renderPass = std::make_shared<RenderPass>(serviceLocator);
      // Create skinned pipeline
      _pipelines["SkinnedPipeline"] =
     std::make_shared<SkinnedPipeline>(_renderPass, serviceLocator, skinnedVert,
     skinnedFrag); _pipelines["CubeMapPipeline"] =
     std::make_shared<CubeMapPipeline>(_renderPass, serviceLocator, cubemapVert,
     cubemapFrag); _pipelines["Pipeline2D"] =
     std::make_shared<Pipeline2D>(_renderPass, serviceLocator, twodVert,
     twodFrag); Setup(serviceLocator);
      */
}
#endif
Renderer::Renderer(std::shared_ptr<ServiceLocator> serviceLocator,
                   flecs::world *world, float xscale, float yscale)

{
  _queueSync = serviceLocator->GetService<QueueSync>();
  _world = world;
  _physicalDevice = serviceLocator->GetService<PhysicalDevice>();
  // Create renderpass
  _renderPass = std::make_shared<RenderPass>(serviceLocator);
  // Create skinned pipeline
  _pipelines["StaticPipeline"] = std::make_shared<StaticPipeline>(
      _renderPass, serviceLocator, VK_POLYGON_MODE_FILL);
  _pipelines["SkinnedPipeline"] = std::make_shared<SkinnedPipeline>(
      _renderPass, serviceLocator, VK_POLYGON_MODE_FILL);
  _pipelines["CubeMapPipeline"] = std::make_shared<CubeMapPipeline>(
      _renderPass, serviceLocator, VK_POLYGON_MODE_FILL);
  _pipelines["Pipeline2D"] = std::make_shared<Pipeline2D>(
      _renderPass, serviceLocator, VK_POLYGON_MODE_FILL);
  _pipelines["GUIPipeline"] = std::make_shared<GUIPipeline>(
      _renderPass, serviceLocator, VK_POLYGON_MODE_FILL);
  _pipelines["GizmoPipeline"] = std::make_shared<GizmoPipeline>(
      _renderPass, serviceLocator, VK_POLYGON_MODE_FILL);
  _pipelines["LinePipeline"] = std::make_shared<LinePipeline>(
      _renderPass, serviceLocator, VK_POLYGON_MODE_LINE);
  Setup(serviceLocator, xscale, yscale);

  _timer = new Timer(1.0);

  /*
  _world->system<Entropy::Components::Renderable>()
      .each([this](flecs::entity e, Entropy::Components::Renderable r)
            {
              if(r.shouldBeDeleted)
              {
                  if(e.is_valid())
                  {
                      spdlog::info("Destroying marked entities");
                      e.destruct();
                      return;
                  }
              }
              });
   */

  _indexBuffer.resize(2);
  _vertexBuffer.resize(2);
}

void Renderer::HandleResize(int width, int height) {
  // ZoneScopedN("Resizing");
  //_synchronizer = std::make_unique<Synchronizer>(
  //    MAX_CONCURRENT_FRAMES_IN_FLIGHT, _serviceLocator);
  _swapChain->RecreateSwapChain();
  _renderPass->RecreateDepthBuffer();
  _renderPass->RecreateFrameBuffers();
}

void Renderer::Wireframe(bool on) {
  _pipelines["SkinnedPipeline"] = std::make_shared<SkinnedPipeline>(
      _renderPass, _serviceLocator,
      on == true ? VK_POLYGON_MODE_LINE : VK_POLYGON_MODE_FILL);

  for (size_t i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++) {
    std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = rawUniformBuffers[i];
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(UniformBufferObject);

    VkDescriptorBufferInfo bufferInfo2{};
    bufferInfo2.buffer = dynUbos[i]->GetVulkanBuffer();
    bufferInfo2.offset = 0;
    bufferInfo2.range = sizeof(UboDataDynamic);

    descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[0].dstSet =
        _pipelines["SkinnedPipeline"]->descriptorSets[0]->Get()[i];
    descriptorWrites[0].dstBinding = 0;
    descriptorWrites[0].dstArrayElement = 0;
    descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrites[0].descriptorCount = 1;
    descriptorWrites[0].pBufferInfo = &bufferInfo;

    descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[1].dstSet =
        _pipelines["SkinnedPipeline"]->descriptorSets[0]->Get()[i];
    descriptorWrites[1].dstBinding = 1;
    descriptorWrites[1].dstArrayElement = 0;
    descriptorWrites[1].descriptorType =
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    descriptorWrites[1].descriptorCount = 1;
    descriptorWrites[1].pBufferInfo = &bufferInfo2;

    vkUpdateDescriptorSets(_logicalDevice->Get(),
                           static_cast<uint32_t>(descriptorWrites.size()),
                           descriptorWrites.data(), 0, nullptr);
  }
}

void Renderer::Render(int width, int height, float xscale, float yscale) {
  _world->progress();

  auto currentFence = _synchronizer->GetFences()[_currentFrame];

  vkWaitForFences(_logicalDevice->Get(), 1, &currentFence, VK_TRUE, UINT64_MAX);

  if (_queueSync->commandBuffers.size() > 0) {
    ZoneScopedN("Submitting Queued commandbuffers!");
    spdlog::info("submitting cmdbuf");
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = _queueSync->commandBuffers.size();
    submitInfo.pCommandBuffers = _queueSync->commandBuffers.data();
    vkQueueSubmit(_logicalDevice->GetPresentQueue(), 1, &submitInfo, nullptr);
    _queueSync->commandBuffers.clear();
  }

  // Reset fences
  vkResetFences(_logicalDevice->Get(), 1, &currentFence);

  auto acquire_result = vkAcquireNextImageKHR(
      _logicalDevice->Get(), _swapChain->Get(), UINT64_MAX,
      _synchronizer->GetImageSemaphores()[_currentFrame], VK_NULL_HANDLE,
      &imageIndex);

  VkResult submit_result = VK_SUCCESS;

  if (acquire_result == VK_SUCCESS || acquire_result == VK_SUBOPTIMAL_KHR) {
    // current commandbuffer
    currentCmdBuffer = _commandBuffers[_currentFrame]->GetCommandBuffer();

    vkResetCommandBuffer(currentCmdBuffer, 0);

    // Begin renderpass and commandbuffer recording
    _commandBuffers[_currentFrame]->Record();
    _renderPass->Begin(_commandBuffers[_currentFrame], imageIndex);

    _world->each<Entropy::Components::Renderable>(
        [this](flecs::entity e, Entropy::Components::Renderable r) {
          DrawEntity(e, r.id);
        });

    // End renderpass and commandbuffer recording
    _renderPass->End(_commandBuffers[_currentFrame]);
    _commandBuffers[_currentFrame]->EndRecording();

    submit_result = SubmitAndPresent(currentCmdBuffer, imageIndex);

    _currentFrame = (_currentFrame + 1) % MAX_CONCURRENT_FRAMES_IN_FLIGHT;
  }

  if (acquire_result != VK_SUCCESS || submit_result != VK_SUCCESS) {
    HandleResize(width, height);
  }
}

void Renderer::DrawEntity(flecs::entity entity, uint32_t index) {
  auto position_component = entity.get_ref<Entropy::Components::Position>();
  auto scale_component = entity.get_ref<Entropy::Components::Scale>();
  auto renderable_component = entity.get_ref<Entropy::Components::Renderable>();
  auto color_component = entity.get_ref<Entropy::Components::Color>();
  auto rotation_component = entity.get_ref<Entropy::Components::Rotation>();

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

  // if (renderable->type == 4)
  // {
  // }
  // else
  // {
  //     rotate = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f),
  //     glm::vec3(0.0, 1.0, 0.0));
  // }

  // auto modelRotation = glm::rotate(glm::mat4(1.0f),
  // glm::radians(renderable->rotation), renderable->orientation);

  UboDataDynamic ubodyn{};

  // if (renderable->type == 0 || renderable->type == 1 || renderable->type ==
  // 3)
  // {
  //     ubodyn.view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f,
  //     0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); ubodyn.proj =
  //     glm::ortho(0.0f, (float)_swapChain->swapChainExtent.width,
  //     (float)_swapChain->swapChainExtent.height, 0.0f, -1.0f, 1.0f);
  //     ubodyn.proj[1][1] *= -1;
  // }
  // else
  // {
  //     ubodyn.view = _cam->GetViewMatrix();
  //     ubodyn.invView = glm::inverse(_cam->GetViewMatrix());
  //     ubodyn.proj = _camera->matrices.perspective;
  //     _camera->update(0.1);
  // }

  // ubodyn.view = _cam->GetViewMatrix();
  //  ubodyn.invView = glm::inverse(_cam->GetViewMatrix());
  //  ubodyn.proj = _camera->matrices.perspective;
  ubodyn.model = translate * rotation * scaling;
  // ubodyn.shapeId = renderable_component->type;
  // ubodyn.color = color_component.get() == nullptr ?
  // glm::vec4(1.0, 1.0, 1.0, 1.0) : color_component->color; ubodyn.time =
  // _timer->GetTick();

  uint32_t offset = dynamicAlignment * index;
  memcpy((char *)dynUbos[_currentFrame]->GetMappedMemory() + offset, &ubodyn,
         sizeof(UboDataDynamic));

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = {_swapChain->swapChainExtent.width,
                    _swapChain->swapChainExtent.height};
  vkCmdSetScissor(currentCmdBuffer, 0, 1, &scissor);

  // Set Viewport
  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = (float)_swapChain->swapChainExtent.width;
  viewport.height = (float)_swapChain->swapChainExtent.height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(currentCmdBuffer, 0, 1, &viewport);

  if (entity.has<Entropy::Components::Model>()) {
    auto ds0 =
        _pipelines["SkinnedPipeline"]->descriptorSets[0]->Get()[_currentFrame];
    vkCmdBindPipeline(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                      _pipelines["SkinnedPipeline"]->GetPipeline());
    vkCmdBindDescriptorSets(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                            _pipelines["SkinnedPipeline"]->GetPipelineLayout(),
                            0, 1, &ds0, 1, &offset);
    auto model = entity.get_ref<Entropy::Components::Model>();

    for (auto node : model->model->nodes) {
      model->model->renderNode(node, currentCmdBuffer,
                               _pipelines["SkinnedPipeline"],
                               Material::ALPHAMODE_OPAQUE);
    }
  }

  if (entity.has<Entropy::Components::OBJModel>()) {
    auto model = entity.get<Entropy::Components::OBJModel>();
    auto ds0 =
        _pipelines["StaticPipeline"]->descriptorSets[0]->Get()[_currentFrame];
    vkCmdBindPipeline(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                      _pipelines["StaticPipeline"]->GetPipeline());
    vkCmdBindDescriptorSets(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                            _pipelines["StaticPipeline"]->GetPipelineLayout(),
                            0, 1, &ds0, 1, &offset);
    vkCmdBindDescriptorSets(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                            _pipelines["StaticPipeline"]->GetPipelineLayout(),
                            1, 1, &model->model->ds, 0, nullptr);

    // Bind vertex & index buffers
    // Bind descriptor sets
    VkBuffer vertexBuffers[] = {model->model->vertexBuffer->GetVulkanBuffer()};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(currentCmdBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdDraw(currentCmdBuffer, model->model->vertices.size(), 1, 0, 0);
  }

  if (entity.has<Entropy::Components::Gizmo>()) {
    auto gizmo = entity.get<Entropy::Components::Gizmo>();
    auto ds0 =
        _pipelines["GizmoPipeline"]->descriptorSets[0]->Get()[_currentFrame];
    vkCmdBindPipeline(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                      _pipelines["GizmoPipeline"]->GetPipeline());
    vkCmdBindDescriptorSets(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                            _pipelines["GizmoPipeline"]->GetPipelineLayout(), 0,
                            1, &ds0, 1, &offset);

    VkBuffer vertexBuffers[] = {gizmo->plane->vertexBuffer->GetVulkanBuffer()};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(currentCmdBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdDraw(currentCmdBuffer, 6, 1, 0, 0);
  }

  if (entity.has<Entropy::Components::LineGizmo>()) {
    auto gizmo = entity.get<Entropy::Components::LineGizmo>();
    auto ds0 =
        _pipelines["LinePipeline"]->descriptorSets[0]->Get()[_currentFrame];
    vkCmdBindPipeline(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                      _pipelines["LinePipeline"]->GetPipeline());
    vkCmdBindDescriptorSets(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                            _pipelines["LinePipeline"]->GetPipelineLayout(), 0,
                            1, &ds0, 1, &offset);

    VkBuffer vertexBuffers[] = {gizmo->line->vertexBuffer->GetVulkanBuffer()};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(currentCmdBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdDraw(currentCmdBuffer, 2, 1, 0, 0);
  }

  /*
  if (renderable->type == 1 || renderable->type == 3)
  {
      auto ds0 =
  _pipelines["Pipeline2D"]->descriptorSets[0]->Get()[_currentFrame];

      vkCmdBindPipeline(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
  _pipelines["Pipeline2D"]->GetPipeline());
      vkCmdBindDescriptorSets(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
  _pipelines["Pipeline2D"]->GetPipelineLayout(), 0, 1, &ds0, 1, &offset); auto
  sprite = std::dynamic_pointer_cast<Sprite>(renderable);
      vkCmdBindDescriptorSets(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
  _pipelines["Pipeline2D"]->GetPipelineLayout(), 1, 1, &sprite->_descriptorSet,
  0, nullptr);
  }

  if (renderable->type == 5)
  {
      auto ds0 =
  _pipelines["GizmoPipeline"]->descriptorSets[0]->Get()[_currentFrame];
      vkCmdBindPipeline(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
  _pipelines["GizmoPipeline"]->GetPipeline());
      vkCmdBindDescriptorSets(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
  _pipelines["GizmoPipeline"]->GetPipelineLayout(), 0, 1, &ds0, 1, &offset);

      VkBuffer vertexBuffers[] = {renderable->vertexBuffer->GetVulkanBuffer()};
      VkDeviceSize offsets[] = {0};
      vkCmdBindVertexBuffers(currentCmdBuffer, 0, 1, vertexBuffers, offsets);
      vkCmdDraw(currentCmdBuffer, 6, 1, 0, 0);
      return;
  }

  if (renderable->type == 0)
  {
      auto ds0 =
  _pipelines["Pipeline2D"]->descriptorSets[0]->Get()[_currentFrame];

      vkCmdBindPipeline(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
  _pipelines["Pipeline2D"]->GetPipeline());
      vkCmdBindDescriptorSets(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
  _pipelines["Pipeline2D"]->GetPipelineLayout(), 0, 1, &ds0, 1, &offset); auto
  sprite = std::dynamic_pointer_cast<Quad>(renderable);
      vkCmdBindDescriptorSets(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
  _pipelines["Pipeline2D"]->GetPipelineLayout(), 1, 1, &sprite->_descriptorSet,
  0, nullptr);
  }

  if (renderable->type == 4)
  {
      auto ds0 =
  _pipelines["SkinnedPipeline"]->descriptorSets[0]->Get()[_currentFrame];
      vkCmdBindPipeline(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
  _pipelines["SkinnedPipeline"]->GetPipeline());
      vkCmdBindDescriptorSets(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
  _pipelines["SkinnedPipeline"]->GetPipelineLayout(), 0, 1, &ds0, 1, &offset);
      auto model = std::dynamic_pointer_cast<Model>(renderable);
      for (auto node : model->nodes)
      {
          model->renderNode(node, currentCmdBuffer,
  _pipelines["SkinnedPipeline"], Material::ALPHAMODE_OPAQUE);
      }
  }
  else if (renderable->type == 10)
  {
      // vkCmdBindPipeline(currentCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
  _pipelines["CubeMapPipeline"]->GetPipeline());
      // auto ds =
  _pipelines["CubeMapPipeline"]->descriptorSets[0]->Get()[_currentFrame];
      // vkCmdBindDescriptorSets(currentCmdBuffer,
  VK_PIPELINE_BIND_POINT_GRAPHICS,
  _pipelines["CubeMapPipeline"]->GetPipelineLayout(), 0, 1,
  &currentDescriptorSet, 1, &offset);
      // auto model = std::dynamic_pointer_cast<CubeMap>(renderable);
      // model->_model->renderNode(model->_model->linearNodes[2],
  currentCmdBuffer, _pipelines["CubeMapPipeline"], Material::ALPHAMODE_OPAQUE);
  }
  else
  {
      // Bind vertex & index buffers
      // Bind descriptor sets
      VkBuffer vertexBuffers[] = {renderable->vertexBuffer->GetVulkanBuffer()};
      VkDeviceSize offsets[] = {0};
      vkCmdBindVertexBuffers(currentCmdBuffer, 0, 1, vertexBuffers, offsets);
      vkCmdBindIndexBuffer(currentCmdBuffer,
  renderable->indexBuffer->GetVulkanBuffer(), 0, VK_INDEX_TYPE_UINT16);
      // Draw current renderable
      vkCmdDrawIndexed(currentCmdBuffer, renderable->GetIndices().size(), 1, 0,
  0, 0);
  }
  */
}

VkResult Renderer::SubmitAndPresent(VkCommandBuffer cmdBuffer,
                                    uint32_t imageIndex) {
  ZoneScopedN("Submitting & Presenting");
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
  if (vkQueueSubmit(_logicalDevice->GetPresentQueue(), 1, &submitInfo,
                    _synchronizer->GetFences()[_currentFrame]) != VK_SUCCESS) {
    exit(EXIT_FAILURE);
  }

  // PresentInfo
  VkPresentInfoKHR presentInfo{};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = signalSemaphores;

  // SwapChains
  VkSwapchainKHR swapChains[] = {_swapChain->Get()};
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapChains;
  presentInfo.pImageIndices = &imageIndex;

  // Present
  auto retval =
      vkQueuePresentKHR(_logicalDevice->GetPresentQueue(), &presentInfo);

  return retval;
}
