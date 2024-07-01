
#pragma once

#include <kangaru/kangaru.hpp>
#include <graphics/vulkan/renderpasses/renderpass.hpp>
#include "swapchain_service.hpp"
#include "vulkan_backend_service.hpp"
#include "texture_factory_service.hpp"

using namespace Entropy::Graphics::Vulkan::RenderPasses;

namespace Entropy {
namespace Graphics {
namespace Services {

struct RenderPassService
    : kgr::single_service<RenderPass, kgr::dependency<VulkanBackendService, SwapchainService, TextureFactoryService>> {};

} // namespace Services
} // namespace Graphics
} // namespace Entropy
