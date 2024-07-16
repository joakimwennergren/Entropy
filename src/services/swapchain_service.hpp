
#pragma once

#include <graphics/vulkan/swapchains/swapchain.hpp>
#include <kangaru/kangaru.hpp>
#include <services/texture_factory_service.hpp>
#include <services/vulkan_backend_service.hpp>

using namespace Entropy::Graphics::Vulkan::Swapchains;

namespace Entropy {
namespace Graphics {
namespace Services {

struct SwapchainService
    : kgr::single_service<Swapchain, kgr::dependency<VulkanBackendService>> {};

} // namespace Services
} // namespace Graphics
} // namespace Entropy
