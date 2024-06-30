
#pragma once

#include "graphics/vulkan/devices/logical_device.hpp"
#include "kangaru/service.hpp"
#include "services/allocator_service.hpp"
#include "services/logical_device_service.hpp"
#include "services/physical_device_service.hpp"
#include "services/vk_instance_service.hpp"
#include <graphics/vulkan/synchronization/synchronizer.hpp>
#include <graphics/vulkan/vulkan_backend.hpp>
#include <kangaru/kangaru.hpp>

using namespace Entropy::Graphics::Vulkan::Synchronization;

namespace Entropy {
namespace Graphics {
namespace Services {

struct VulkanBackendService
    : kgr::single_service<
          Vulkan::VulkanBackend,
          kgr::dependency<VulkanInstanceService, PhysicalDeviceService,
                          LogicalDeviceService, AllocatorService>> {};

} // namespace Services
} // namespace Graphics
} // namespace Entropy
