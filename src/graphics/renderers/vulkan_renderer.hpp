#pragma once

#include "cameras/camera_manager.hpp"
#include "config.hpp"
#include "ecs/components/position.hpp"
#include "graphics/vulkan/memory/allocator.hpp"
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <data/ubo.hpp>
#include <ecs/world.hpp>
#include <graphics/vulkan/buffers/stagedbuffer.hpp>
#include <graphics/vulkan/buffers/storagebuffer.hpp>
#include <graphics/vulkan/buffers/uniformbuffer.hpp>
#include <graphics/vulkan/commandbuffers/commandbuffer.hpp>
#include <graphics/vulkan/pipelines/static_pipeline.hpp>
#include <graphics/vulkan/renderpasses/renderpass.hpp>
#include <graphics/vulkan/swapchains/swapchain.hpp>
#include <graphics/vulkan/synchronization/synchronizer.hpp>
#include <obj/model.hpp>
#include <cameras/orthographic_camera.hpp>

#include <ecs/components/renderable.hpp>

#include <flecs.h>
#include <timing/timer.hpp>
//#include <tracy/Tracy.hpp>

#ifdef BUILD_FOR_ANDROID
#include <android/asset_manager.h>
#endif

using namespace Entropy::Graphics::Vulkan::Buffers;
using namespace Entropy::Graphics::Vulkan::Textures;
using namespace Entropy::Graphics::Vulkan::Pipelines;
using namespace Entropy::Graphics::Vulkan::RenderPasses;
using namespace Entropy::Graphics::Vulkan::CommandBuffers;
using namespace Entropy::Graphics::Vulkan::Synchronization;
using namespace Entropy::Graphics::Vulkan::DescriptorSets;
using namespace Entropy::Graphics::Vulkan::SwapChains;
using namespace Entropy::Graphics::Vulkan::Synchronization;
using namespace Entropy::Data;
using namespace Entropy::ECS;
using namespace Entropy::Cameras;

namespace Entropy::Graphics::Vulkan::Renderers {
    struct VulkanRenderer {
        VulkanRenderer() {
            const ServiceLocator *sl = ServiceLocator::GetInstance();

            _allocator = sl->getService<IAllocator>();
            _logicalDevice = sl->getService<ILogicalDevice>();
            _swapchain = sl->getService<ISwapchain>();
            _world = sl->getService<IWorld>();
            _cameraManager = sl->getService<ICameraManager>();

            _renderPass = std::make_shared<RenderPass>();
            //_renderPass->CreateFrameBuffers(800, 800);

            // Static Pipeline creation
            _staticPipeline = std::make_unique<StaticPipeline>(_renderPass);

            _synchronizer = std::make_unique<Synchronizer>(MAX_CONCURRENT_FRAMES_IN_FLIGHT);

            for (uint32_t i = 0; i < MAX_CONCURRENT_FRAMES_IN_FLIGHT; i++) {
                _commandBuffers.emplace_back(VK_COMMAND_BUFFER_LEVEL_PRIMARY);
            }

            _UBO = std::make_unique<UniformBuffer>(sizeof(UboDataDynamic));
            _instanceDataBuffer = std::make_unique<StorageBuffer>(
                MAX_INSTANCE_COUNT * sizeof(InstanceData));

            // Update descriptor sets
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
                descriptorWrites[0].dstSet = _staticPipeline->descriptorSet;
                descriptorWrites[0].dstBinding = 1;
                descriptorWrites[0].dstArrayElement = 0;
                descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                descriptorWrites[0].descriptorCount = 1;
                descriptorWrites[0].pBufferInfo = &bufferInfo;

                descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptorWrites[1].dstSet = _staticPipeline->descriptorSet;
                descriptorWrites[1].dstBinding = 0;
                descriptorWrites[1].dstArrayElement = 0;
                descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
                descriptorWrites[1].descriptorCount = 1;
                descriptorWrites[1].pBufferInfo = &objectBufferInfo;

                vkUpdateDescriptorSets(_logicalDevice->Get(),
                                       static_cast<uint32_t>(descriptorWrites.size()),
                                       descriptorWrites.data(), 0, nullptr);
            }

            // Create a query for the Position component with a custom sorting function
            _sortQuery = _world->Get()
                    ->query_builder<Components::Position>()
                    .order_by<Components::Position>(compare_zIndex)
                    .build();

            _timer = std::make_unique<Timing::Timer>(1.0f);
            _timer->Start();
        }

        ~VulkanRenderer() = default;

        void Render(int width, int height,
                    float xscale, float yscale);

        // Comparison function for sorting based on z index
        static int compare_zIndex(flecs::entity_t e1, const Components::Position *p1,
                                  flecs::entity_t e2,
                                  const Components::Position *p2) {
            return (p1->pos.z > p2->pos.z) - (p1->pos.z < p2->pos.z);
        }

        void PresentForApplication() const {
            // End render pass and command buffer recording
            RenderPass::End(_commandBuffers[_currentFrame]);
            _commandBuffers[_currentFrame].EndRecording();

            const auto cmdBuffer = _commandBuffers[_currentFrame].Get();

            // Submit info
            VkSubmitInfo submitInfo{};
            const VkSemaphore signalSemaphores[] = {
                _synchronizer->GetRenderFinishedSemaphores()[_currentFrame]
            };
            const VkSemaphore waitSemaphores[] = {
                _synchronizer->GetImageSemaphores()[_currentFrame]
            };
            VkPipelineStageFlags waitStages[] = {
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
            };

            std::vector submittables = {cmdBuffer};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.waitSemaphoreCount = 1;
            submitInfo.pWaitSemaphores = waitSemaphores;
            submitInfo.pWaitDstStageMask = waitStages;
            submitInfo.commandBufferCount = submittables.size();
            submitInfo.pCommandBuffers = submittables.data();
            submitInfo.signalSemaphoreCount = 1;
            submitInfo.pSignalSemaphores = signalSemaphores;

            // Submit queue
            if (vkQueueSubmit(_logicalDevice->GetGraphicQueue(), 1, &submitInfo,
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
            VkSwapchainKHR swapChains[] = {_swapchain->Get()};
            presentInfo.swapchainCount = 1;
            presentInfo.pSwapchains = swapChains;
            presentInfo.pImageIndices = &imageIndex;

            // Present
            vkQueuePresentKHR(_logicalDevice->GetGraphicQueue(), &presentInfo);
        }

        void OnResize(const int width, const int height) {
            _synchronizer =
                    std::make_unique<Synchronizer>(MAX_CONCURRENT_FRAMES_IN_FLIGHT);
            _swapchain->RecreateSwapChain(width, height);
            _renderPass->RecreateDepthBuffer(width, height);
            _renderPass->RecreateFrameBuffers(width, height);
        }

        std::shared_ptr<RenderPass> _renderPass;
        std::shared_ptr<ISwapchain> _swapchain;

    private:
        flecs::query<Components::Position> _sortQuery;

        // Container to hold entities and their Position components
        std::vector<flecs::entity> _sortedEntities;

        // Custom comparator function for sorting
        static bool CompareZIndex(const flecs::entity &a, const flecs::entity &b) {
            return a.get_ref<Components::Position>()->pos.z < b.get_ref<Components::Position>()->pos.z;
            // Sort by zIndex
        }

        std::unique_ptr<Timing::Timer> _timer;

        uint32_t _currentFrame = 0;
        uint32_t imageIndex = 0;

        // Command Buffers
        std::vector<CommandBuffer> _commandBuffers;

        // UBO
        std::unique_ptr<UniformBuffer> _UBO;

        std::unique_ptr<StorageBuffer> _instanceDataBuffer;
        // Pipelines
        std::unique_ptr<StaticPipeline> _staticPipeline;
        // Synchronizer
        std::unique_ptr<Synchronizer> _synchronizer;

        // Dependencies
        std::shared_ptr<IAllocator> _allocator;
        std::shared_ptr<ILogicalDevice> _logicalDevice;
        std::shared_ptr<IWorld> _world;
        std::shared_ptr<ICameraManager> _cameraManager;
    };
} // namespace Entropy::Graphics::Vulkan::Renderers
