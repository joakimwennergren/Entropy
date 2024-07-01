
#pragma once

#include <kangaru/kangaru.hpp>
#include <factories/vulkan/bufferfactory.hpp>
#include <services/vulkan_backend_service.hpp>
#include <services/renderpass_service.hpp>
#include <services/swapchain_service.hpp>
#include <services/descriptor_pool_service.hpp>

using namespace Entropy::Factories::Vulkan;

namespace Entropy {
namespace Graphics {
namespace Services {

struct DescriptorSetLayoutFactoryService
    : kgr::single_service<DescriptorSetLayoutFactory, kgr::dependency<VulkanBackendService>> {};

} // namespace Services
} // namespace Graphics
} // namespace Entropy
