#pragma once

#include "services/vk_instance_service.hpp"
#include <graphics/devices/device.hpp>
#include <kangaru/kangaru.hpp>

namespace Entropy {
namespace Graphics {
namespace Services {
struct PhysicalDeviceService
    : kgr::single_service<PhysicalDevice,
                          kgr::dependency<VulkanInstanceService>> {};
} // namespace Services
} // namespace Graphics
} // namespace Entropy
