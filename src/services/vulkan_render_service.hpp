
#pragma once

#include "graphics/renderers/vulkan_renderer.hpp"
#include "graphics/vulkan/devices/logical_device.hpp"
#include "kangaru/service.hpp"
#include "services/allocator_service.hpp"
#include "services/buffer_factory_service.hpp"
#include "services/logical_device_service.hpp"
#include "services/physical_device_service.hpp"
#include "services/pipeline_factory_service.hpp"
#include "services/queue_sync_service.hpp"
#include "services/renderpass_service.hpp"
#include "services/vk_instance_service.hpp"
#include "services/vulkan_backend_service.hpp"
#include "services/command_pool_service.hpp"
#include "services/swapchain_service.hpp"
#include <graphics/renderers/renderer.hpp>
#include <graphics/vulkan/synchronization/synchronizer.hpp>
#include <graphics/vulkan/vulkan_backend.hpp>
#include <kangaru/kangaru.hpp>

using namespace Entropy::Graphics::Vulkan::Synchronization;
using namespace Entropy::Graphics::Renderers;

namespace Entropy {
namespace Graphics {
namespace Services {

struct VulkanRenderService
    : kgr::service<
          VulkanRenderer,
          kgr::dependency<VulkanBackendService, QueueSyncService,
                          RenderPassService, PipelineFactoryService,
                          BufferFactoryService, CommandPoolService, SwapchainService>> {};

} // namespace Services
} // namespace Graphics
} // namespace Entropy
