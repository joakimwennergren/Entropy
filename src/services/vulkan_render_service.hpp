
#pragma once

#include "graphics/devices/logical_device.hpp"
#include "graphics/renderers/vulkan_renderer.hpp"
#include "kangaru/service.hpp"
#include "services/allocator_service.hpp"
#include "services/logical_device_service.hpp"
#include "services/physical_device_service.hpp"
#include "services/vk_instance_service.hpp"
#include "services/vulkan_backend_service.hpp"
#include "vulkan_backend.hpp"
#include <graphics/renderers/renderer.hpp>
#include <graphics/vulkan/synchronization/synchronizer.hpp>
#include <kangaru/kangaru.hpp>

using namespace Entropy::Graphics::Vulkan::Synchronization;
using namespace Entropy::Graphics::Renderers;

namespace Entropy {
namespace Graphics {
namespace Services {

struct VulkanRenderService
    : kgr::single_service<VulkanRenderer,
                          kgr::dependency<VulkanBackendService>> {};

} // namespace Services
} // namespace Graphics
} // namespace Entropy
