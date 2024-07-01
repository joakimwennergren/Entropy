
#pragma once

#include <kangaru/kangaru.hpp>
#include <factories/vulkan/descriptorset_factory.hpp>
#include <services/vulkan_backend_service.hpp>
#include <services/renderpass_service.hpp>
#include <services/swapchain_service.hpp>
#include <services/descriptor_pool_service.hpp>

using namespace Entropy::Factories::Vulkan;

namespace Entropy {
namespace Graphics {
namespace Services {

struct DescriptorSetFactoryService
    : kgr::single_service<DescriptorSetFactory, kgr::dependency<VulkanBackendService, DescriptorPoolService>> {};

} // namespace Services
} // namespace Graphics
} // namespace Entropy
