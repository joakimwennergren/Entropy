
#pragma once

#include "kangaru/service.hpp"
#include "services/vulkan_backend_service.hpp"
#include <kangaru/kangaru.hpp>

using namespace Entropy::Graphics::Vulkan::CommandPools;

namespace Entropy {
namespace Graphics {
namespace Services {

struct CommandPoolService
    : kgr::service<CommandPool, kgr::dependency<VulkanBackendService>> {};

} // namespace Services
} // namespace Graphics
} // namespace Entropy
