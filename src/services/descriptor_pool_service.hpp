
#pragma once

#include "kangaru/service.hpp"
#include "services/vulkan_backend_service.hpp"
#include <graphics/vulkan/descriptorpools/descriptorpool.hpp>
#include <kangaru/kangaru.hpp>

using namespace Entropy::Graphics::Vulkan::DescriptorPools;

namespace Entropy {
namespace Graphics {
namespace Services {

struct DescriptorPoolService : kgr::service<DescriptorPool, kgr::dependency<VulkanBackendService>> {
};

} // namespace Services
} // namespace Graphics
} // namespace Entropy
