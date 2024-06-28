
#pragma once

#include "services/logical_device_service.hpp"
#include "services/physical_device_service.hpp"
#include <graphics/devices/device.hpp>
#include <kangaru/kangaru.hpp>

namespace Entropy {
namespace Graphics {
namespace Services {

struct DeviceService
    : kgr::single_service<Device, kgr::dependency<PhysicalDeviceService>> {};

} // namespace Services
} // namespace Graphics
} // namespace Entropy
