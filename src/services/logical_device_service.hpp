#pragma once

#include "services/physical_device_service.hpp"
#include <graphics/vulkan/devices/logical_device.hpp>
#include <kangaru/kangaru.hpp>

namespace Entropy {
namespace Graphics {
namespace Services {
struct LogicalDeviceService
    : kgr::single_service<LogicalDevice,
                          kgr::dependency<PhysicalDeviceService>> {};
} // namespace Services
} // namespace Graphics
} // namespace Entropy
