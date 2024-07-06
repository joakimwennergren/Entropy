
#pragma once

#include "services/pipelinecache_service.hpp"
#include <factories/vulkan/bufferfactory.hpp>
#include <kangaru/kangaru.hpp>
#include <services/descriptor_pool_service.hpp>
#include <services/descriptor_set_factory_service.hpp>
#include <services/descriptor_set_layout_factory_service.hpp>
#include <services/renderpass_service.hpp>
#include <services/swapchain_service.hpp>
#include <services/vulkan_backend_service.hpp>

using namespace Entropy::Factories::Vulkan;

namespace Entropy {
namespace Graphics {
namespace Services {

struct PipelineFactoryService
    : kgr::single_service<
          PipelineFactory,
          kgr::dependency<VulkanBackendService, RenderPassService,
                          SwapchainService, DescriptorPoolService,
                          DescriptorSetLayoutFactoryService,
                          DescriptorSetFactoryService, PipelineCacheService>> {
};

} // namespace Services
} // namespace Graphics
} // namespace Entropy
