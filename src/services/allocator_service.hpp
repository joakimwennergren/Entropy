
#pragma once

#include "kangaru/service.hpp"
#include "services/logical_device_service.hpp"
#include "services/physical_device_service.hpp"
#include "services/vk_instance_service.hpp"
#include <graphics/memory/allocator.hpp>
#include <kangaru/kangaru.hpp>

using namespace Entropy::Graphics::Memory;

namespace Entropy {
namespace Graphics {
namespace Services {

struct AllocatorService
    : kgr::single_service<Allocator, kgr::dependency<VulkanInstanceService,
                                                     PhysicalDeviceService,
                                                     LogicalDeviceService>> {};

} // namespace Services
} // namespace Graphics
} // namespace Entropy
